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

static uid_t granted_users[] = { 3889 };
static size_t num_granted_users = 1;

int main(int argc, char** argv, char** envp)
{
    int i;
    int granted = 0;
    uid_t uid;
    if(argc < 2)
    {
        fprintf(stderr, "Error: Missing arguments\n");
        fprintf(stderr, "Usage: %s command [arg]...\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    uid = getuid();
    for(i = 0; i < num_granted_users; i ++)
    {
        if(uid == granted_users[i])
        {
            granted = 1;
            break;
        }
    }

    if(!granted)
    {
        fprintf(stderr, "You are nasty!\n");
        exit(1);
    }
    execute(argv[1], &(argv[1]));

    return EXIT_SUCCESS;
}
