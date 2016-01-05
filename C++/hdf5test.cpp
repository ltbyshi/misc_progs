#include <iostream>
using namespace std;
#include <H5Cpp.h>
#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif

#define FILENAME "test.h5"
#define DATASETNAME "data"

void WriteDataSet()
{
    hsize_t dims[2] = {1000, 1000};
    hsize_t count = dims[0]*dims[1];

    H5File h5file(FILENAME, H5F_ACC_TRUNC);
    DataSpace dataspace(2, dims);
    IntType datatype(PredType::NATIVE_INT);
    DataSet dataset = h5file.createDataSet(DATASETNAME, datatype, dataspace);
    int* data = new int[count];
    for(hsize_t i = 0; i < count; i ++)
        data[i] = i;
    dataset.write(data, PredType::NATIVE_INT);
    dataset.close();
    h5file.close();
}

int main()
{
    WriteDataSet();
    return 0;
}
