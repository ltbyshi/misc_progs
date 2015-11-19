#include <stdio.h>

#define SIZE 1024*1024*512
unsigned char data[SIZE];

int main()
{
    unsigned int sum = 0;
    for(int i = 0; i < SIZE; i ++)
        sum += data[i];
    printf("%u\n", sum);

    return 0;
}
