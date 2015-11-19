#include <stdio.h>

void CountZeros()
{
    unsigned long a;
    int lz[64], tz[64];
    int i;

    for(i = 0; i < 64; i ++)
    {
        a = (1L << i);
        lz[i] = __builtin_clzl(a);
        tz[i] = __builtin_ctzl(a);
    }
    printf("Leading zeros: ");
    for(i = 0; i < 64; i ++)
        printf("%d ", lz[i]);
    printf("\n");
    
    printf("Trailing zeros: ");
    for(i = 0; i < 64; i ++)
        printf("%d ", tz[i]);
    printf("\n");
}

void Trap()
{
    __builtin_trap();
}

int main()
{
    CountZeros();
    Trap();
    return 0;
}
