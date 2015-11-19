#include <iostream>
using namespace std;
#include <stdlib.h>

int main()
{
    float* pf = new float;
    unsigned int* pi = (unsigned int*)pf;
    *pi = 0;
    cout << 2 /(*pi) << endl;
    while(true)
    {
        *pi = rand();
        *pf *= 2.0;
    }
    cout << *pf << endl;
    return 0;
}
