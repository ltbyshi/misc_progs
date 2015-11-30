#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    int status, fd;

    fd = shm_open("segfault", O_CREAT, 0);
    if(fd == -1)
    {
        perror("shm_open");
        exit(-1);
    }
    return 0;
}

