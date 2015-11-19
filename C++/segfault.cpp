#include <stdio.h>

int main()
{
    char* p = (char*)printf;
    for(int i = 0; i < 1024; i ++)
        printf("%d ", (int)p[i]);
    printf("\n");
    return 0;
}
