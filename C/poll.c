#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFSIZE (1UL << 24)

int fd_stdin;
int fd_stdout;
int poll_next = 1;

void PollFile()
{
    fd_stdin = fileno(stdin);
    fd_stdout = fileno(stdout);
    while(poll_next)
    {
        int status;
        struct pollfd fds[2];

        fds[0].fd = fd_stdin;
        fds[1].events = POLLIN | POLLOUT | POLLPRI;
        fds[1].fd = fd_stdout;
        fds[1].events = POLLIN | POLLOUT | POLLPRI;
        status = poll(&(fds[0]), 2, 200);
        if(status > 0)
        {
            int i;
            for(i = 0; i < 2; i ++)
            {
                fprintf(stderr, "fd: %d, events: ", fds[i].fd);
                if(fds[i].revents & POLLIN) fprintf(stderr, "POLLIN ");
                if(fds[i].revents & POLLOUT) fprintf(stderr, "POLLOUT ");
                if(fds[i].revents & POLLPRI) fprintf(stderr, "POLLPRI ");
                if(fds[i].revents & POLLHUP) fprintf(stderr, "POLLHUP ");
                if(fds[i].revents & POLLERR) fprintf(stderr, "POLLERR ");
                if(fds[i].revents & POLLNVAL) fprintf(stderr, "POLLNVAL ");
                fprintf(stderr, "\n");
            }
        }
        else if(status == -1)
        {
            perror("poll");
        }

    }
}

void* PollFileThread(void* args)
{
    PollFile();
    pthread_exit(NULL);
}

void DoIO()
{
    char* buf;
    buf = (char*)malloc(BUFSIZE);
    fclose(stdout);
    fgets(buf, 10240, stdin);
    puts(buf);
    free(buf);
    sleep(1);
}

int main()
{
    pthread_t tid;
    if(pthread_create(&tid, NULL, PollFileThread, NULL) != 0)
    {
        fprintf(stderr, "pthread_create failed\n");
        exit(-1);
    }
    DoIO();
    poll_next = 0;
    pthread_join(tid, NULL);

    return 0;
}

