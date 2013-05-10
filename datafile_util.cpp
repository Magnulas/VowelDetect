#include <fstream>
#include <iostream>
#include <iomanip>
#include <ios>

struct Data* readDataFromFile(char* filename);

struct Data{
	double** data;
	int pdim;
	int nsamples;
};

struct Data* readDataFromFile(char* filename){
	FILE * f = fopen(filename, "r");
	int nsamples = 0;
	int pdim = 0;
	fscanf(f, "%d %d\n", &nsamples, &pdim );
	double p;
	double** data = (double**)malloc(sizeof(double*)*nsamples);
	
	for(int i = 0;i<nsamples;i++){
		data[i] = (double*) malloc(sizeof(double)*pdim);
		for(int j = 0;j<pdim;j++){
			fscanf(f, "%lf",&p);
			data[i][j] = p;
		}
	}	

	struct Data* datastruct = (Data*) malloc(sizeof(Data));
	datastruct->data=data;
	datastruct->pdim = pdim;
	datastruct->nsamples=nsamples;
	return datastruct;
}
