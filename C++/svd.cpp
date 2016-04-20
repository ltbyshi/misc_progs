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
    if(argc != 4)
    {
        cerr << "Usage: " << argv[0] << " nrow ncol p" << endl;
        exit(1);
    }
    long nrow, ncol, p;
    if(!StringToLong(argv[1], nrow) 
            || !StringToLong(argv[2], ncol)
            || !StringToLong(argv[3], p))
    {
        cerr << "Error: nrow or ncol or p is not valid" << endl;
        exit(1);
    }
    if((p > nrow) || (p > ncol))
    {
        cerr << "Error: p should be smaller than nrow or ncol" << endl;
        exit(1);
    }
    cout << "nrow=" << nrow << ", ncol=" << ncol << endl;
    size_t n_display = 5;
    MatrixXd m(nrow, ncol);
    srand(time(0));
    m.setRandom();
    cout << "Original matrix (" << m.rows() << ", " << m.cols() << "):" << endl;
    HeadMatrix(m, n_display, n_display);
    JacobiSVD<MatrixXd> svd(m, ComputeThinV | ComputeThinU);
    cout << "Singular values (" << svd.singularValues().size() << "):" << endl;
    HeadVector(svd.singularValues(), n_display);
    cout << "Matrix U(" << svd.matrixU().rows() << ", " << svd.matrixU().cols() << "):" << endl;
    HeadMatrix(svd.matrixU(), n_display, n_display);
    cout << "Matrix V(" << svd.matrixV().rows() << ", " << svd.matrixV().cols() << "):" << endl;
    HeadMatrix(svd.matrixV(), n_display, n_display);
    
    DiagonalMatrix<double, Dynamic> s(p);
    s.diagonal() = svd.singularValues().head(p);
    MatrixXd ut(svd.matrixU().leftCols(p));
    MatrixXd vt(svd.matrixV().leftCols(p).transpose());
    cout << "Matrix Ut(" << ut.rows() << ", " << ut.cols()
        << "), Matrix V't(" << vt.rows() << ", " << vt.cols()
        << "), S(" << s.rows() << ", " << s.cols() << ")" << endl;
    MatrixXd t = ut * s * vt;
    HeadMatrix(t, n_display, n_display);

    return 0;
}
