#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int main(int argc, char** argv)
{
    int i, j, retry;
    char c;

    srand(time(0));
    for(i = 0; i < 20; i ++)
    {
        c = (char)(rand()%(0x7A - 0x30 + 1) + 0x30);
        retry = rand() % 10;
        for(j = 0; j < retry; j ++)
        {
            while(!isalnum(c))
                c = (char)(rand()%(0x7A - 0x30 + 1) + 0x30);
        }
        printf("%c", c);
    }
    printf("\n");
    return 0;
}