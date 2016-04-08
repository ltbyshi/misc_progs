#include <Eigen/Dense>
#include <Eigen/SVD>
using namespace Eigen;
#include <iostream>
using namespace std;
#include <stdlib.h>

bool StringToLong(const char* s, long& val, int base = 10)
{
    char *endptr;
    long v = strtol(s, &endptr, base);
    bool status = ((errno == ERANGE) && (v == LONG_MAX || v == LONG_MIN))
            || (errno != 0 && val == 0)
            || (endptr == s);
    status = !status;
    if(status)
        val = v;
    return status;
}

template <typename MatrixType>
void HeadMatrix(const MatrixType m, size_t nrow, size_t ncol)
{
    for(size_t i = 0; i < nrow; i ++)
    {
        for(size_t j = 0; j < ncol; j ++)
            cout << m(i, j) << " ";
        cout << endl;
    }
}

template <typename VectorType>
void HeadVector(const VectorType v, size_t n)
{
    for(size_t i = 0; i < n; i ++)
        cout << v(i) << " ";
    cout << endl;
}

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        cerr << "Usage: " << argv[0] << " nrow ncol" << endl;
        exit(1);
    }
    long nrow, ncol;
    if(!StringToLong(argv[1], nrow) || !StringToLong(argv[2], ncol))
    {
        cerr << "Error: nrow or ncol is not valid" << endl;
        exit(1);
    }
    cout << "nrow=" << nrow << ", ncol=" << ncol << endl;
    size_t n_display = 5;
    MatrixXd m(nrow, ncol);
    srand(time(0));
    m.setRandom();
    JacobiSVD<MatrixXd> svd(m, ComputeThinV | ComputeThinU);
    cout << "Singular values:" << endl;
    HeadVector(svd.singularValues(), n_display);
    cout << "Matrix U:" << endl;
    HeadMatrix(svd.matrixU(), n_display, n_display);
    cout << "Matrix V:" << endl;
    HeadMatrix(svd.matrixV(), n_display, n_display);
    
    return 0;
}
