#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

const size_t maxbytes = 10;
char msg[1024];

int main(int argc, char** argv)
{
    size_t i;
    FILE* fp;
    int fd;
    struct stat filestat;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(1);
    }
    fp = fopen(argv[1], "rb+");
    if(!fp)
    {
        sprintf(msg, "Error: cannot open the file %s", argv[1]);
        perror(msg);
        exit(1);
    }
    fd = fileno(fp);
    fstat(fd, &filestat);
    srand(time(0));
    for(i = 0; i < maxbytes; i ++)
    {
        fseek(fp, rand() % filestat.st_size, SEEK_SET);
        fputc(rand() % 0xFF, fp);
    }
    fclose(fp);

    return 0;
}
