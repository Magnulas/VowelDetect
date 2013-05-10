struct Data* readDataFromFile(char* filename);
struct Data{
	double** data;
	int pdim;
	int nsamples;
};
