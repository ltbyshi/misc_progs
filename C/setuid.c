#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute(const char* path, char** argv)
{
    pid_t pid;
   
    pid = fork();
    if(pid < 0)
    {
        perror("Error: fork() failed");
        exit(-1);
    }
    else if(pid == 0)
    {
        if(execvp(path, argv) < 0)
        {
            perror("Error: execvp() failed");
            exit(-1);
        }
    }
    else
    {
        int status;
        while(wait(&status) != pid) {}
    }
}

int main(int argc, char** argv, char** envp)
{
    //system("bash");
    //execl("/bin/bash", "bash", NULL);
    if(argc < 2)
    {
        fprintf(stderr, "Error: Missing arguments\n");
        fprintf(stderr, "Usage: %s command [arg]...\n", argv[0]);
        return EXIT_FAILURE;
    }
    printf("uid=%d, gid=%d, euid=%d, egid=%d\n",
            getuid(), getgid(), geteuid(), getegid());
    if(setuid(geteuid()) == -1)
        perror("Error: setuid() failed");
    if(setgid(getegid()) == -1)
        perror("Error: setgid() failed");
    execute(argv[1], &(argv[1]));
    /*
    if(execvp(argv[1], &(argv[1])) == -1)
    {
        perror("Error: cannot execute the command");
        return EXIT_FAILURE;
    }
    */
    return EXIT_SUCCESS;
}
