#include <stdio.h>
#include <errno.h>

int main()
{
    int filenum = 0;
    while(fopen("/dev/zero", "r"))
        filenum ++;
    perror("Cannot open more files");
    fprintf(stderr, "Opened files: %d\n", filenum);
    return 0;
}
