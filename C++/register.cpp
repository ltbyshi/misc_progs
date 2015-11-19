#include <stdio.h>

int main()
{
    register int a[100];
    for(register int i = 0; i < 100; i ++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}
