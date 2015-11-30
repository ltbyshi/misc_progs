#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>

void _ZN10FisherTestC1Ei(int signum) 
{
    long addr;
    addr = random();
    *((int*)addr) = 0;
    // raise(SIGSEGV);
}

int main()
{
    struct itimerval tm;
    tm.it_interval.tv_sec = 0;
    tm.it_interval.tv_usec = 100000;
    tm.it_value.tv_sec = 0;
    tm.it_value.tv_usec = 100000;

    srandom(time(0));
    signal(SIGALRM, _ZN10FisherTestC1Ei);
    setitimer(ITIMER_REAL, &tm, NULL);
    sleep(100);
    return 0;
}
