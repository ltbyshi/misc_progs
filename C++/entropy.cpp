#include <vector>
#include <iostream>
using namespace std;

#include <stdlib.h>
#include <time.h>
#include <math.h>

inline double Entropy(const std::vector<double>& x)
{
    double sum = 0;
    for(size_t i = 0; i < x.size(); i ++)
        sum += x[i];
    double result = 0;
    for(size_t i = 0; i < x.size(); i ++)
    {
        double xn = x[i]/sum;
        result += xn*log(xn);
    }
    result = 1.0 + result/log(x.size());
    return result;
}

int main()
{
    srand48(time(0));
    for(int i = 0; i < 10000; i ++)
    {
        vector<double> x(6);
        for(int j = 0; j < 6; j ++)
        {
            x[j] = drand48();
            cout << x[j] << "\t";
        }
        cout << Entropy(x) << endl;
    }

    return 0;
}
