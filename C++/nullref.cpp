#include <iostream>
using namespace std;

template <typename T>
T& NullRef()
{
    return *((T*)0);
}

int main()
{
    int& n = NullRef<int>();
    cout << n << endl;
    int& c = n;
    cout << c << endl;
    return 0;
}
