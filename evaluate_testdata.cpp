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

int main(int argc, char **argv, char **envp)
{
	if(argc%2==0||argc<3){
		fprintf(stderr,"Please specify pairs of gmm and test data. The testdata should be the posetives for that gmm. The other testdata will be used as negatives.\n");
		return 0;
	} 

	int nGmms = argc/2;
	char* test_data[nGmms];
	char* gmm_files[nGmms];
	for(int i = 0;i<nGmms;i++){
		test_data[i] = argv[2*i+2];
		gmm_files[i] = argv[2*i+1];
	}


	MOG_diag gmm[nGmms];
	int classifications[nGmms][nGmms];
	for(int i = 0;i<nGmms;i++){
		for(int j = 0;j<nGmms;j++){
			classifications[i][j] = 0;
		}
	}

	for(int j = 0;j<nGmms;j++){
		gmm[j].load(gmm_files[j]);
	}

	int D = gmm[0].get_means()(0).size();
	int ntotalSamples = 0;
	int wrongClassifed = 0;
	vec datasample(D);
	for(int i = 0;i<nGmms;i++){
			
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
	cout << endl;
	for(int i = 0;i<nGmms;i++){
		sscanf(gmm_files[i],"%*[a-zA-Z0-9]/%[A-Za-z0-9].gmm", buffer);
		cout << buffer;
		for(int j = 0;j<nGmms;j++){
			cout << "\t" << classifications[i][j];
		}
		cout << endl;
	}

	return 0;
}
