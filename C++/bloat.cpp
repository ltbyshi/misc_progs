#include <stdio.h>
#include <stdlib.h>
#define N (1L << 20)


inline static void loops()
{
    long int a[N], b[N], c[N];

    for(int i = 0; i < N; i ++)
        a[i] = rand();
    for(int i = 0; i < N; i ++)
        b[i] = a[i] + 1;
    for(int i = 0; i < N; i ++)
        c[i] = a[i] + b[i];
    for(int i = 0; i < N; i ++)
        printf("%ld ", c[i]);
    printf("\n");
}

inline static long getn(long n)
{
    printf("%ld\n", n);
    if(n > 0)
        return getn(n - 1);
    else
        return 0;
}

int main()
{
    for(int i = 0; i < (1L << 14); i ++)
        loops();
    long n = getn((1L << 10));
    printf("%ld\n", n);
    return 0;
}

