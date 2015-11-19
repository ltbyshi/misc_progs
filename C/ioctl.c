#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

char msg[10240];

int main(int argc, char** argv)
{
    int  fd, i;
    unsigned long req;
    const char* filename;
    unsigned int* data;
    size_t datasize;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(-1);
    }
    filename = argv[1];
    fd = open(filename, 0);
    if(fd == -1)
    {
        sprintf(msg, "open(%s)", filename);
        perror(msg);
        exit(-1);
    }
    datasize = 1024*sizeof(unsigned int);
    data = (unsigned int*)malloc(datasize);
    memset(data, 0, datasize);
    srand(time(0));
    for(i = 0; i < 1024; i ++)
        data[i] = rand();

    for(req = 0; req < 0x1000000; req ++)
    {
        if(ioctl(fd, req, data) == -1)
        {
            // sprintf(msg, "ioctl(%08X)", req);
            // perror(msg);
        }
        else
        {
            printf("ioctl(%08luX): Success\n", req);
        }
    }

    return 0;
}
