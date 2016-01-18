#include <iostream>
using namespace std;
#include <stdlib.h>
#include <string.h>
#ifndef H5_NO_NAMESPACE
#include <H5Cpp.h>
using namespace H5;
#endif
#include <unistd.h>

#define DATASETNAME "data"

void WriteDataSet(const char* filename)
{
    hsize_t dims[2] = {5000, 5000};
    hsize_t count = dims[0]*dims[1];

    cout << "Delete " << filename << endl;
    unlink(filename);
    H5File h5file(filename, H5F_ACC_TRUNC);
    DataSpace dataspace(2, dims);
    IntType datatype(PredType::NATIVE_INT);
    DataSet dataset = h5file.createDataSet(DATASETNAME, datatype, dataspace);
    
    int attr_int = 16;
    int attr_float = 3.14;
    H5std_string attr_string("hello");
    DataSpace scalar_space;
    StrType strtype(PredType::C_S1, H5T_VARIABLE);
    dataset.createAttribute("int", PredType::NATIVE_INT, scalar_space)
        .write(PredType::NATIVE_INT, &attr_int);
    dataset.createAttribute("float", PredType::NATIVE_FLOAT, scalar_space)
        .write(PredType::NATIVE_FLOAT, &attr_float);
    dataset.createAttribute("string", strtype, scalar_space)
        .write(strtype, attr_string);
    cout << "Attribute 'int': " << attr_int << endl;
    cout << "Attribute 'float': " << attr_float << endl;
    cout << "Attribute 'string': " << attr_string << endl;
        
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
    
    int attr_int;
    int attr_float;
    H5std_string attr_string;
    StrType strtype(PredType::C_S1, H5T_VARIABLE);
    dataset.openAttribute("int").read(PredType::NATIVE_INT, &attr_int);
    dataset.openAttribute("float").read(PredType::NATIVE_FLOAT, &attr_float);
    dataset.openAttribute("string").read(strtype, attr_string);
    cout << "Attribute 'int': " << attr_int << endl;
    cout << "Attribute 'float': " << attr_float << endl;
    cout << "Attribute 'string': " << attr_string << endl;
    
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

void SelectData(const char* filename)
{
    H5File h5file(filename, H5F_ACC_RDONLY);
    DataSet dataset = h5file.openDataSet(DATASETNAME);
    DataSpace filespace = dataset.getSpace();
    int rank = filespace.getSimpleExtentNdims();
    hsize_t dims[2];
    filespace.getSimpleExtentDims(dims, NULL);
    
    hsize_t nelements = (dims[1] > dims[0])? dims[0] : dims[1];
    hsize_t* coords = new hsize_t[nelements*2];
    for(hsize_t i = 0, j = 0; i < nelements; i += 2, j ++)
    {
        coords[i] = j;
        coords[i + 1] = j;
    }
    
    filespace.selectElements(H5S_SELECT_SET, nelements, coords);
    hsize_t memdims[1];
    memdims[0] = nelements;
    DataSpace memspace(1, memdims);
    int* data = new int[nelements];
    dataset.read(data, PredType::NATIVE_INT, memspace, filespace);
    cout << "Data: ";
    for(hsize_t i = 0; i < 10; i ++)
        cout << " " << data[i];
    cout << endl;
    dataset.close();
    h5file.close();
    
    delete[] coords;
    delete[] data;
}

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        cerr << "Usage: " << argv[0] << " command filename" << endl;
        cerr << "Available commands: read write select" << endl;
        exit(1);
    }
    const char* command = argv[1];
    const char* filename = argv[2];
    if(!strcmp(command, "read"))
        ReadDataSet(filename);
    else if(!strcmp(command, "write"))
        WriteDataSet(filename);
    else if(!strcmp(command, "select"))
        SelectData(filename);
    else
    {
        cerr << "Error: invalid command" << endl;
        exit(1);
    }
    return 0;
}
