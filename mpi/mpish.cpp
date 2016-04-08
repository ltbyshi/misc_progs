#include <iostream>
using namespace std;
#include <MPIUtil.h>
using MPIUtil::mpienv;
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void SignalHandler(int signum)
{
    switch(signum)
    {
        case SIGPIPE:
            fprintf(stderr, "Broken pipe");
    }
    exit(signum);
}

void ConnectStream(int fdin, int fdout, size_t bufsize=1)
{
    char* buf = new char[bufsize];
    FILE* fin = fdopen(fdin, "rb");
    FILE* fout = fdopen(fdout, "wb");
    while(true)
    {
        size_t rsize = fread(buf, 1, bufsize, fin);
        size_t wsize = fwrite(buf, 1, rsize, fout);
        fflush(fout);
        //fprintf(stderr, "write %lu bytes\n", wsize);
        if((rsize < bufsize) || (wsize < bufsize) || feof(fin))
            break;
    }
    fclose(fin);
    fclose(fout);
    delete[] buf;
}

void WriteMessage(int fd)
{
    const char* msg = "Write into pipe\n";
    FILE* fout = fdopen(fd, "wb");
    fwrite(msg, 1, strlen(msg), fout);
    fclose(fout);
}

void StartShell()
{
    int pipe_stdin[2];
    int pipe_stdout[2];
    pid_t pid_child;
    
    pipe(pipe_stdin);
    pipe(pipe_stdout);

    if((pid_child = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }
    if(pid_child == 0)
    {
        // child
        //close(0);
        close(pipe_stdin[1]);
        close(pipe_stdout[0]);
        close(0);
        if(dup2(pipe_stdin[0], 0) == -1)
        {
            perror("dup2");
            exit(255);
        }
        //dup2(1, pipe_stdout[1]);
        execlp("/bin/bash", "/bin/bash", NULL);
    }
    else
    {
        // parent
        close(pipe_stdin[0]);
        close(pipe_stdout[1]);
        int status;
        
        //dup2(0, pipe_stdin[1]);
        //dup2(1, pipe_stdin[1]);
        //dup2(0, pipe_stdout[0]);
        ConnectStream(0, pipe_stdin[1]);
        //WriteMessage(pipe_stdin[1]);
        waitpid(pid_child, &status, 0);
        fprintf(stderr, "Shell exited with %d\n", status);
    }
}

int main()
{
    signal(SIGPIPE, SignalHandler);

    StartShell();
    return 0;
}
