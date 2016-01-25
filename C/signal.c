#include <signal.h>
#include <stdio.h>

int main()
{
    FILE* f;
    unsigned int ran;
    int signum;
    f = fopen("/dev/urandom", "rb");
    fread(&ran, sizeof(ran), 1, f);
    signum = ran % SIGRTMAX + 1;
    while(signum == SIGSTOP || signum == SIGTRAP || signum == SIGTSTP 
          || signum == SIGTTIN || signum == SIGTTOU || signum == SIGINT)
    {
        fread(&ran, sizeof(ran), 1, f);
        signum = ran % SIGRTMAX + 1;
    }
    fclose(f);
    printf("raise(%d)\n", signum);
    fflush(stdout);
    raise(signum);
    return 0;
}
