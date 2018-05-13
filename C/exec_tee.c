#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <signal.h>

int main(int argc, char** argv, char** envp)
{
    int pipe_stdout[2];
    pid_t cpid;
    char buf;
    char error_prompt[128];
    char** exec_argv;
    int i;
    
    FILE* fout[1];
    char filename[1][PATH_MAX];

    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s output_dir command [arguments ...]\n");
        exit(EXIT_FAILURE);
    }
    if(pipe(pipe_stdout) == -1)
    {
        perror("error in pipe()");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if(cpid == -1)
    {
        perror("error in fork()");
        exit(EXIT_FAILURE);
    }

    // child process
    if(cpid == 0)
    {
        // stdin => pipe
        dup2(pipe_stdout[0], STDIN_FILENO);
        close(pipe_stdout[0]);
        close(pipe_stdout[1]);
        exec_argv = (char**)malloc(sizeof(char*)*(argc));
        for(i = 0; i < argc - 1; i ++)
        {
            exec_argv[i] = (char*)malloc(1 + strlen(argv[i + 1]));
            strcpy(exec_argv[i], argv[i + 1]);
        }
        exec_argv[argc - 1] = NULL;

        execv(argv[1], exec_argv);
        perror("execv() error");
        exit(EXIT_FAILURE);
    }
    // parent process
    else
    {
        sprintf(filename[0], "exec_tee.%d", (int)cpid);
        fout[0] = fopen(filename[0], "w");
        if(!fout[0])
        {
            sprintf(error_prompt, "fopen(%s) failed", filename[0]);
            perror(error_prompt);
            killpg(0, SIGTERM);
        }
        close(pipe_stdout[0]);
        while(read(STDIN_FILENO, &buf, 1) > 0)
            write(pipe_stdout[1], &buf, 1);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
    return 0;
}