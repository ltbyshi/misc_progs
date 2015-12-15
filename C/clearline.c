#include <stdio.h>
#include <time.h>
#include <string.h>
#define PROGRESSBAR_MAX 50

int main()
{
    struct timespec ts = {.tv_sec = 0, .tv_nsec = 10000000};
    int i, j;
    char progress_bar[PROGRESSBAR_MAX + 1];

    memset(progress_bar, ' ', PROGRESSBAR_MAX);
    progress_bar[PROGRESSBAR_MAX] = 0;   
    for(i = 0; i < 1000; i ++)
    {
        double progress;

        nanosleep(&ts, NULL);
        progress = (double)(i + 1)/1000;
        for(j = 0; j < (int)(progress*PROGRESSBAR_MAX); j ++)
            progress_bar[j] = '=';
        printf("\r[%s] %3.1lf%%", progress_bar, progress*100);
        fflush(stdout);
    }
    printf("\n");

    return 0;
}

    
