#include "Statistics.h"

void Factorial(int n, long double* f)
{
    f[0] = 1;
    f[1] = 1;
    for(int i = 2; i <= n; i ++)
        f[i] = i*f[i - 1];
}
