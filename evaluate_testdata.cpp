#include <itpp/itstat.h>
#include <float.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ios>
#include "datafile_util.h"
using std::cout;
using std::endl;
using std::fixed;
using std::setprecision;

using namespace itpp;

void createConfusionMatrix(int nGmms, int nDataFiles,char* test_data[],MOG_diag gmm[],bool,double,char* gmm_files[]);

int main(int argc, char **argv, char **envp)
{
	if(argc<3){
		fprintf(stderr,"Please specify pairs of gmm and test data. The testdata should be the posetives for that gmm. The other testdata will be used as negatives. Optionally specify noise data as  last argument.\n");
		return 0;
	} 

	bool useProbThresh = false;
	double probCeil = 0.02;
	double probStep = 0.02;
	double probstart = 0.00001;
	if(argc%2==0){
		useProbThresh = true;
	}

	int nGmms = (argc-1)/2;
	int nDataFiles = argc/2;
	char* test_data[nDataFiles];
	char* gmm_files[nGmms];
	for(int i = 0;i<nGmms;i++){
		gmm_files[i] = argv[2*i+2];
	}

	for(int i = 0;i<nDataFiles;i++){
		test_data[i] = argv[2*i+1];
	}

	MOG_diag gmm[nGmms];
	
	for(int j = 0;j<nGmms;j++){
		gmm[j].load(gmm_files[j]);
	}

	if(useProbThresh){
		for(double probthreashold=probstart;probthreashold<probCeil;probthreashold+=probStep){
			cout << "Treashold: " << probthreashold << endl;	
		
			createConfusionMatrix(nGmms, nDataFiles,test_data,gmm,useProbThresh,probthreashold,gmm_files);
		}
	} else{
		createConfusionMatrix(nGmms, nDataFiles,test_data,gmm,useProbThresh,0,gmm_files);
	}
	return 0;
}

void createConfusionMatrix(int nGmms, int nDataFiles,char* test_data[],MOG_diag gmm[],bool useProbThresh,double probthreashold, char* gmm_files[]){


	int D = gmm[0].get_means()(0).size();
	int ntotalSamples = 0;
	int wrongClassifed = 0;
	vec datasample(D);
	int classifications[nDataFiles][nDataFiles];
	for(int i = 0;i<nDataFiles;i++){
		for(int j = 0;j<nDataFiles;j++){
			classifications[i][j] = 0;
		}
	}

	for(int i = 0;i<nDataFiles;i++){
			
		struct DataMatrix* data = readDataMatrixFromFile(test_data[i]);
		
		for(int k=0;k<data->nsamples;k++){
			double likelihood = DBL_MIN;
			int index = -1;
			for(int j = 0;j<nGmms;j++){
				for(int d=0;d<D;d++){
					datasample(d) = data->data[k][d];
				}
	

				double tmp = gmm[j].lhood(datasample);
				if(likelihood<tmp){
					likelihood = tmp;
					index=j;
				}
			}
			if(useProbThresh&&likelihood<probthreashold)
				index=nDataFiles-1;
			
			classifications[i][index]++;
			if(i!=index){
				wrongClassifed++;
			}
			
		}
		ntotalSamples+=data->nsamples;
		//free
		for(int k = 0;k<data->nsamples;k++){
			free(data->data[k]);
		}
		free(data->data);
		free(data);
	}

	double correct = 100*(ntotalSamples-wrongClassifed)/(double)ntotalSamples;
	fprintf(stdout,"Correct: %.2lf%% NSamples: %d Wrong: %d\n",correct,ntotalSamples,wrongClassifed);

	char buffer[50];
	for(int i = 0;i<nGmms;i++){
		sscanf(gmm_files[i],"%*[a-zA-Z0-9]/%[A-Za-z0-9].gmm", buffer);
		cout << "\t" << buffer;
	}
	if(useProbThresh){
		cout << "\tNOISE";
	}

	cout << endl;
	int tp = 0;
	int tn = 0;
	int fp = 0;
	int fn = 0;
	for(int i = 0;i<nGmms;i++){
		sscanf(gmm_files[i],"%*[a-zA-Z0-9]/%[A-Za-z0-9].gmm", buffer);

		cout << buffer;
		for(int j = 0;j<nDataFiles;j++){
			cout << "\t" << classifications[i][j];
			if(j<nDataFiles-1){
				tp += classifications[i][j];
			} else{
				fp += classifications[i][j];
			}			
		}
		

		cout << endl;
	}
	if(useProbThresh){
		cout << "NOISE";
		for(int j = 0;j<nDataFiles;j++){
			cout << "\t" << classifications[nDataFiles-1][j];

			if(j<nDataFiles-1){
				fn += classifications[nDataFiles-1][j];
			} else{
				tn += classifications[nDataFiles-1][j];
			}
		}
		cout << endl;
	}

	cout << fixed << setprecision(4);
	double tpr = 100*((double)tp)/(double)(tp+fn);
	double fpr = 100*((double)fp)/(double)(tn+fp);

	if(useProbThresh){
		cout << "Tp: " << tp << endl;
		cout << "Fp: " << fp << endl;
		cout << "Tn: " << tn << endl;
		cout << "Fn: " << fn << endl;
		cout << "Tpr: " << tpr << endl;
		cout << "Fpr: " << fpr << endl;
	}
}
