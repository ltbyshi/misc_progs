#include <iostream>
using namespace std;

template <int N>
int OrdinalSum()
{
    return N*(N + 1)/2;
}

#define PRINT_ORDINALSUM(n) \
    cout << "OrdinalSum(" << n << ") = " << OrdinalSum<n>() << endl;
int main()
{
    PRINT_ORDINALSUM(1);
    PRINT_ORDINALSUM(2);
    PRINT_ORDINALSUM(3);
    PRINT_ORDINALSUM(4);
    PRINT_ORDINALSUM(5);
    PRINT_ORDINALSUM(6);
    return 0;
}
