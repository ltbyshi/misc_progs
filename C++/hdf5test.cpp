#include <iostream>
using namespace std;
#include <stdlib.h>
#include <string.h>
#include <H5Cpp.h>
#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif

#define DATASETNAME "data"

void WriteDataSet(const char* filename)
{
    hsize_t dims[2] = {1024, 500000};
    hsize_t count = dims[0]*dims[1];

    H5File h5file(filename, H5F_ACC_TRUNC);
    DataSpace dataspace(2, dims);
    IntType datatype(PredType::NATIVE_INT);
    DataSet dataset = h5file.createDataSet(DATASETNAME, datatype, dataspace);
    int* data = new int[count];
    for(hsize_t i = 0; i < count; i ++)
        data[i] = i;
    dataset.write(data, PredType::NATIVE_INT);
    delete[] data;
    dataset.close();
    h5file.close();
}

void ReadDataSet(const char* filename)
{
    H5File h5file(filename, H5F_ACC_RDONLY);
    DataSet dataset = h5file.openDataSet(DATASETNAME);
    if(dataset.getTypeClass() == H5T_INTEGER) 
    {
        IntType datatype = dataset.getIntType();
        cout << "DataType.Order = " << datatype.getOrder() << endl;
        cout << "DataType.Size = " << datatype.getSize() << endl;
    }
    DataSpace dataspace = dataset.getSpace();
    int rank = dataspace.getSimpleExtentNdims();
    hsize_t dims[2];
    dataspace.getSimpleExtentDims(dims, NULL);
    cout << "DataSpace.Rank = " << rank << endl;
    cout << "DataSpace.Dim = " << dims[0] << "," << dims[1] << endl;
    hsize_t count = dims[0]*dims[1];
    int* data = new int[count];
    dataset.read(data, PredType::NATIVE_INT);
    cout << "Data:";
    for(hsize_t i = 0; i < 10; i ++)
        cout << " " << data[i];
    cout << endl;
    delete[] data;
    dataset.close();
    h5file.close();
}

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        cerr << "Usage: " << argv[0] << " command filename" << endl;
        cerr << "Available commands: read write" << endl;
        exit(1);
    }
    const char* command = argv[1];
    const char* filename = argv[2];
    if(!strcmp(command, "read"))
        ReadDataSet(filename);
    else if(!strcmp(command, "write"))
        WriteDataSet(filename);
    else
    {
        cerr << "Error: invalid command" << endl;
        exit(1);
    }
    return 0;
}
