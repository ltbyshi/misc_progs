#include <stdio.h>

class A
{
public:
    A()
    {
        *((long*)this) = 0;
    }
    short a;
};

A a;
int main()
{
    printf("%d\n", a.a);
}

