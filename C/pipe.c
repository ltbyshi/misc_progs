#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int pipefd[2];
    const char* msg = "hello";

    if(pipe(pipefd) == -1)
        perror("pipe");
    if(close(pipefd[0]) == -1)
        perror("close");
    while(1)
    {
        if(write(pipefd[1], msg, strlen(msg) == -1))
            perror("write");
    }
    return 0;
}

