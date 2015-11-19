#include <iostream>
using namespace std;


void Factorial(int n, long double* f)
{
    f[0] = 1;
    f[1] = 1;
    for(int i = 2; i <= n; i ++)
        f[i] = i*f[i - 1];
}

class FisherTest
{
public:
    FisherTest(int maxn)
    {
        _maxn = maxn;
        _f = new long double[maxn + 1];
        Factorial(maxn, _f);
    }

    ~FisherTest()
    {
        if(_f) delete[] _f;
    }

    double Density(int x, int m, int n, int k) const
    {
        return (_f[m]*_f[n]*_f[k]*_f[m+n-k]/(_f[x]*_f[m-x]*_f[k-x]*_f[n+x-k]*_f[m+n]));
    }

    double TestGreater(int x, int m, int n, int k) const
    {
        double p = 0;
        for(int ix = x; ix <= k; ix ++)
            p += Density(ix, m, n, k);
        return p;
    }

private:
    int _maxn;
    long double* _f;
};


int main()
{
    int total = 500;
    int m = total*0.6;
    int n = total - m;
    int k = total*0.02;

    FisherTest test(total);
    for(int x = 0; x <= k; x ++)
    {
        printf("Density(%d,%d,%d,%d) = %lg\n", x, m, n, k, test.Density(x, m, n, k));
        printf("TestGreater(%d,%d,%d,%d) = %lg\n", x, m, n, k, test.TestGreater(x, m, n, k));
    }
    return 0;
}
