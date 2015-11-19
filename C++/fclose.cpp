#include <stdio.h>

int main()
{
    FILE* fp = fopen("/dev/zero", "r");
    for(int i = 0; i < 1000000; i ++)
        fclose(fp);

    return 0;
}
