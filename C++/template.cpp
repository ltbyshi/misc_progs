#include <iostream>
using namespace std;
#include <stdlib.h>

template <typename T = int>
T Add(T a, T b)
{
    return (a + b);
}

template <typename T = int>
struct Hash
{
    T operator()(const char* key)
    {
        T v = 0;
        while(*key++)
            v += *key;
        return v;
    }
};

template <int N>
int Series()
{
    return (Series<N - 1>() + Series<N - 2>())/2;
}

template <>
int Series<1>() { return 1; }

template <>
int Series<2>() { return 1; }

int main()
{
    int a = rand();
    int b = rand();
    cout << a << "+" << b << "=" << Add(a, b) << endl;
    
    Hash<int> hash;
    cout << "hash of hello: " << hash("hello") << endl;
    return 0;
}
