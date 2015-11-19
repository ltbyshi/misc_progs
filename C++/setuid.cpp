#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute(char** argv)
{
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("Error in fork");
        exit(-1);
    }
    else if(pid == 0)
    {
        if(execvp(argv[0], argv) < 0)
        {
            perror("Error in execvp");
            exit(-1);
        }
    }
    else
    {
        int status;
        while(wait(&status) != pid) {}
    }
}

int main()
{
    if((setuid(0) == -1) || (setgid(0) ==-1))
        perror("Error in setuid");
    else
    {
        char* argv[2];
        argv[0] = (char*)malloc(255);
        argv[1] = NULL;
        strcpy(argv[0], "/bin/bash");
        execute(argv);
        // execvp("/bin/bash", argv);

    }
    return 0;
}
