#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main()
{
    srand(time(0));
    unsigned long n = rand();
    unsigned long nbits = __builtin_popcountl(n);
    printf("Number of bits in %lu is %lu\n", n, nbits);

    return 0;
}
