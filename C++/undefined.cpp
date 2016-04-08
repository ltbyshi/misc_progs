#include <iostream>
#include <stdlib.h>
using namespace std;

struct Object
{
    int i;
    char& c;
    int a[1000000000];
    int Sum() {
        int r = 0;
        for(size_t i = 0; i < sizeof(a)/sizeof(int); i ++)
            r += a[i];
        return r;
    }
};

template <typename Type>
Type Undefined()
{
    int a = rand();
}

ostream UndefinedOS()
{
    long a = rand();
}

int main()
{
    //UndefinedOS() << "Undefined OS" << endl;
    for(int i = 0; i < 10; i ++)
        cout << "Undefined: " << Undefined<Object>().Sum() << endl;
    return 0;
}
