#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int main()
{
    const char* filename = "/tmp/big";
    const size_t length = (1UL << 62);
    const size_t bufsize = (1UL << 20);
    FILE* fp;
    size_t i;
    char* buf;

    fp = fopen(filename, "wb+");
    if(ftruncate(fileno(fp), length) == -1)
    {
        perror("ftruncate");
        exit(1);
    }
    buf = (char*)malloc(bufsize);
    memset(buf, 0, bufsize);
    for(i = 0; i < length/bufsize; i ++)
    {
        if(fwrite(buf, bufsize, 1, fp) < 1)
        {
            perror("fwrite");
            exit(1);
            break;
        }
    }
    fclose(fp);
    free(buf);

    return 0;
}
