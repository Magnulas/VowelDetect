#include <itpp/itstat.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <ios>

using std::cout;
using std::endl;
using std::fixed;
using std::setprecision;

using namespace itpp;

struct Data* readDataFromFile(char* filename);

struct Data{
	float** data;
	int pdim;
	int nsamples;
};


int main(int argc, char **argv, char **envp)
{
	if(argc<3){
		fprintf(stderr,"Please specify training data and trained gmm output name and test data\n");
		return 0;
	} 

	char* trainingdataname = argv[1];
	char* outputname = argv[2];
	//char* testdataname = argv[3];

	struct Data* data = readDataFromFile(trainingdataname);

	bool print_progress = false;

	int N = data->nsamples;	// number of vectors
	int D = data->pdim;		 // number of dimensions
	int K = 1;		 // number of Gaussians

	Array<vec> X(N);
	for (int n = 0;n < N;n++) { X(n).set_size(D); X(n) = 0.0; }

	// randomise the order of Gaussians "generating" the vectors
	for (int n = 0;n < N;n++) {
		for (int d = 0;d < D;d++) {
			X(n)(d) = data->data[n][d];
		}
	}

	// make a model with initial values (zero mean and unit variance)
	// the number of gaussians and dimensions of the model is specified here

	MOG_diag mog(K, D);

	cout << endl;
	cout << fixed << setprecision(3);
	cout << "mog.avg_log_lhood(X) = " << mog.avg_log_lhood(X) << endl;

	//
	// find initial parameters via k-means (which are then used as seeds for EM based optimisation)

	cout << endl << endl;
	cout << "running kmeans optimiser" << endl << endl;

	MOG_diag_kmeans(mog, X, 10, 0.5, true, print_progress);

	cout << fixed << setprecision(3);
	cout << "mog.get_means() = " << endl << mog.get_means() << endl;
	cout << "mog.get_diag_covs() = " << endl << mog.get_diag_covs() << endl;
	cout << "mog.get_weights() = " << endl << mog.get_weights() << endl;

	cout << endl;
	cout << "mog.avg_log_lhood(X) = " << mog.avg_log_lhood(X) << endl;


	//
	// EM ML based optimisation

	cout << endl << endl;
	cout << "running ML optimiser" << endl << endl;

	//10 iterations of EM
	MOG_diag_ML(mog, X, 10, 0.0, 0.0, print_progress);

	cout << fixed << setprecision(3);
	cout << "mog.get_means() = " << endl << mog.get_means() << endl;
	cout << "mog.get_diag_covs() = " << endl << mog.get_diag_covs() << endl;
	cout << "mog.get_weights() = " << endl << mog.get_weights() << endl;

	cout << endl;
	cout << "mog.avg_log_lhood(X) = " << mog.avg_log_lhood(X) << endl;
	
	mog.save(outputname);
	//it_save_var_as<Array<vec>>(&X, outputname);
	
	return 0;
}

struct Data* readDataFromFile(char* filename){
	FILE * f = fopen(filename, "r");
	//Todo fix multiple dimensions
	int nsamples = 0;
	int pdim = 0;
	fscanf(f, "%d %d\n", &nsamples, &pdim );
	pdim = 3;
	float* p = (float*)malloc(sizeof(float)*pdim);
	int i = 0;
	float** data = (float**)malloc(sizeof(float*)*nsamples);
	
	for(i = 0;i<nsamples;i++){
		data[i] = (float*) malloc(sizeof(float)*pdim);
		fscanf(f, "%f %f %f\n", &p[0],&p[1],&p[2]);
		data[i][0] = p[0];
		data[i][1] = p[1];
		data[i][2] = p[2];
	}	

	struct Data* datastruct = (Data*) malloc(sizeof(Data));
	datastruct->data=data;
	datastruct->pdim = pdim;
	datastruct->nsamples=nsamples;
	return datastruct;
}
