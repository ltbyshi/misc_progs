#include <iostream>
using namespace std;

template <unsigned int n>
struct getn {
    enum { value = 1 + getn<n - 1>::value };
};

template <>
struct getn<0> {
    enum { value = 0 };
};

int main()
{
    cout << getn<100>::value << endl;
    return 0;
}

