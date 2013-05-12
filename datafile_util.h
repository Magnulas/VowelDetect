struct Data* readDataFromFile(char* filename);
struct DataMatrix* readDataMatrixFromFile(char* filename);
struct Data{
	float* data;
	int pdim;
	int nsamples;
};
struct DataMatrix{
	double** data;
	int pdim;
	int nsamples;
};
