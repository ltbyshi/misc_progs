#include <iostream>
using namespace std;

#define UNSIGNED(t) cout << "#t(-1): " << (t)-1 << endl;
int main()
{
    UNSIGNED(unsigned char);
    UNSIGNED(unsigned short);
    UNSIGNED(unsigned int);
    UNSIGNED(unsigned long);

    return 0;
}
