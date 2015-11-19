#include <bitset>
#include <iostream>
using namespace std;
#include <stdlib.h>
#include <time.h>

struct SeedInit
{
    SeedInit() { srand(time(0)); }
};

SeedInit seedinit;

int main()
{
    bitset<1024*1024> bs;
    size_t nbits = rand() % 1000 + 1000;
    for(size_t i = 0; i < nbits; i ++)
        bs[rand() % nbits] = true;
    cout << "nbits: " << nbits
        << ", count: " << bs.count() << endl;
    return 0;
}
