#include <iostream>
using namespace std;

int main()
{
    int& n = *((int*)0);
    cout << n << endl;
}
