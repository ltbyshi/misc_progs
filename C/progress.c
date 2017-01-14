#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

size_t n_lines = 0;
size_t total_lines = 0;
double interval = 1;

void handle_alarm(int signum)
{
    if(signum == SIGALRM){
        total_lines += n_lines;
        fprintf(stderr, "[progress] %lu lines/s, %lu lines in total\n",
                (size_t)((double)n_lines/interval), total_lines);
        n_lines = 0;
    }
}

int main(int argc, char** argv)
{
    int c;
    struct itimerval itv;
    int status;

    itv.it_interval.tv_sec = interval;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = itv.it_interval.tv_sec;
    itv.it_value.tv_usec = 0;
    status = setitimer(ITIMER_REAL, &itv, NULL); 
    if(status != 0){
        perror("setitmer()");
        exit(1);
    }
    signal(SIGALRM, handle_alarm);
    c = getchar();
    while(c != EOF){
        if(c == '\n')
            n_lines ++;
        c = getchar();
    }
}
