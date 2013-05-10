#include <itpp/itstat.h>

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
	if(argc<4){
		fprintf(stderr,"Please specify training data and trained gmm name and the number of mixtuers\n");
		return 0;
	} 
	
	char* trainingdataname = argv[1];
	char* outputname = argv[2];
	int K = atoi(argv[3]); // number of Gaussians

	struct Data* data = readDataFromFile(trainingdataname);

	bool print_progress = false;

	int N = data->nsamples;	// number of vectors
	int D = data->pdim;		 // number of dimensions
	
	Array<vec> X(N);
	for (int n = 0;n < N;n++) { X(n).set_size(D); X(n) = 0.0; }

	// randomise the order of Gaussians "generating" the vectors
	for (int n = 0;n < N;n++) {
		for (int d = 0;d < D;d++) {
			X(n)(d) = data->data[n][d];
		}
	}

	fprintf(stdout, "Output: %s\n", outputname);

	// make a model with initial values (zero mean and unit variance)
	// the number of gaussians and dimensions of the model is specified here

	MOG_diag mog(K, D);
	
	//
	// find initial parameters via k-means (which are then used as seeds for EM based optimisation)

	MOG_diag_kmeans(mog, X, 10, 0.5, true, print_progress);

	cout << "running ML optimiser" << endl;

	//
	// EM ML based optimisation
	//100 iterations of EM
	MOG_diag_ML(mog, X, 100, 0.0, 0.0, print_progress);
	
	cout << fixed << setprecision(3);
	cout << "mog.get_means() = " << endl << mog.get_means() << endl;
	cout << "mog.get_diag_covs() = " << endl << mog.get_diag_covs() << endl;
	cout << "mog.get_weights() = " << endl << mog.get_weights() << endl;

	cout << endl;
	cout << "mog.avg_log_lhood(X) = " << mog.avg_log_lhood(X) << endl;
	mog.save(outputname);
	
	return 0;
}

