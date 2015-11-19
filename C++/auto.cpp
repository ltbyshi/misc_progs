#include <iostream>
#include <stdlib.h>
using namespace std;

int get_int()
{
    return int(rand());
}

int main()
{
    auto a = get_int();
    auto b = 1;
    cout << "auto int: " << a + b  << endl;
    return 0;
}
