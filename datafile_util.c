#include <fstream>
#include <iostream>
#include <iomanip>
#include <ios>
#include <malloc.h>
#include "datafile_util.h"


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
		//	fprintf(stdout, "%lf ", p);
			data[i][j] = p;
		}
		//fprintf(stdout, "\n", p);
	}	

	struct Data* datastruct = (Data*) malloc(sizeof(Data));
	datastruct->data=data;
	datastruct->pdim = pdim;
	datastruct->nsamples=nsamples;
	return datastruct;
}
