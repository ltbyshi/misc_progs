#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int a;

    srand(time(0));
    a = rand()/100;
    printf("%d %d %d %d %d %d\n", a*2, a*4, a*8, a*16, a*32, a*64);
    return 0;
}
