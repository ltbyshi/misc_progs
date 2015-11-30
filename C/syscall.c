#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>

#define DATASIZE 1024*16
#define MAXCALLNO 1024L*1024

char msg[4096];

void SysCall(long callno, char* data)
{
        printf("syscall(%ld)", callno);
        fflush(stdout);
        if(syscall(callno, data) == 0)
            printf(": Success\n");
        else
            printf(": %s\n", strerror(errno));
}

int main(int argc, char** argv)
{
    long callno;
    int i;

    srand(time(0));
    char* data = (char*)malloc(4096);
    for(i = 0; i < DATASIZE; i ++)
        data[i] = rand();
    data[DATASIZE - 1] = 0;

    if(argc == 2)
    {
        char* endptr;
        callno = strtol(argv[1], &endptr, 10);
        SysCall(callno, data);
    }
    else
    {
        for(callno = 0; callno < MAXCALLNO; callno ++)
            SysCall(callno, data);
    }
    return 0;
}
