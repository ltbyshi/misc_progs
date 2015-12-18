#include <iostream>
#include <algorithm>
using namespace std;
#include <stdlib.h>

#define OPT_ARGSORT 0
#define OPT_SORT 1

typedef int DType;
template <typename T>
struct Compare1
{
    bool operator()(T a, T b) { return (a >= b); }
};

template <typename ValType>
struct ArgCompare
{
    ArgCompare(const ValType* v) : v(v) {}
    bool operator()(size_t i, size_t j) { return (v[i] > v[j]); }
    const ValType* v;
};


int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cerr << "Usage: " << argv[0] << " filename" << endl;
        exit(1);
    }
    FILE* fp = fopen(argv[1], "rb");
    if(!fp)
    {
        perror("Error: cannot open the input file");
        exit(1);
    }
    uint64_t typesize;
    uint64_t size;
    fread(&typesize, sizeof(typesize), 1, fp);
    fread(&size, sizeof(size), 1, fp);
    if(typesize != sizeof(DType))
    {
        cerr << "Wrong type size" << endl;
        exit(1);
    }
    cout << "typesize: " << typesize << ", size: " << size << endl;
    DType* data = new DType[size];
    fread((void*)data, typesize, size, fp);
    fclose(fp);

    cout << "Before sorting:" << endl;
    for(int i = 0; i < 10; i ++)
        cout << data[i] << " ";
    cout << endl;
#if OPT_SORT
    sort(data, data + size, std::less<DType>());
    cout << "After sorting:" << endl;
    for(int i = 0; i < 10; i ++)
        cout << data[i] << " ";
    cout << endl;
#endif
#if OPT_ARGSORT
    int* index = new int[size];
    for(size_t i = 0; i < size; i ++)
        index[i] = i;
    sort(index, index + size, ArgCompare<float>(data));
    cout << "After sorting:" << endl;
    for(int i = 0; i < 10; i ++)
        cout << index[i] << " ";
    cout << endl;
    for(int i = 0; i < 10; i ++)
        cout << data[index[i]] << " ";
    cout << endl;
    delete[] index;
#endif
    delete[] data;
    
    return 0;
}

