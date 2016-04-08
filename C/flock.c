#include <stdio.h>
#include <sys/file.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char** argv)
{
    FILE* fp;
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if(!fp)
    {
        perror("Error: cannot open the file");
        exit(1);
    }
    if(flock(fileno(fp), LOCK_EX | LOCK_NB) != 0)
    {
        perror("Error: flock");
        exit(1);
    }
    sleep(UINT_MAX);
    return 0;
}

