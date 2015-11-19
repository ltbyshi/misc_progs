#include<stdio.h>
#include<stdlib.h>
#define SIZE    1024*1024*768
char a[SIZE], b[SIZE], c[SIZE];

int main()
{
    int i;

    for(i = 0; i < SIZE; i ++)
        c[i] = a[i] + b[i];
    printf("%d\n", (int)c[rand()%SIZE]);
    abort();
    return 0;
}
