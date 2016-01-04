#include <stdio.h>
#include <signal.h>
#include <stdint.h>

void handler(int signum)
{
    printf("Received signal %d\n", signum);
    signal(SIGILL, NULL);
}

int main(int argc, char** argv, char** envp)
{
    signal(SIGILL, handler);
    __asm__("jmp main");
    __asm__("int $0x80");
    __asm__("sysenter");
    // __asm__("ud2");
    return 0;
}
