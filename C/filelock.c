#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/cdrom.h>
#include <linux/fd.h>

int main(int argc, char** argv)
{
    FILE* fp;
    struct flock flk;
    int fd;
    int status;
    unsigned int req;
    char msg[1024];

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(-1);
    }

    fp = fopen(argv[1], "rb");
    if(!fp)
    {
        perror("fopen");
        exit(-1);
    }

    fd = fileno(fp);

    for(req = 0; req < 0xFF; req ++)
    {
        if(ioctl(fd, req) == -1)
        {
            sprintf(msg, "ioctl(%08X)", req);
            perror(msg);
        }
    }

    flk.l_type = F_RDLCK;
    flk.l_whence = SEEK_SET;
    flk.l_start = 0;
    flk.l_len = 0;
    status = fcntl(fd, F_SETLK, &flk);
    if(status == -1)
    {
        perror("fcntl");
        exit(-1);
    }
    fclose(fp);
    return 0;
}
