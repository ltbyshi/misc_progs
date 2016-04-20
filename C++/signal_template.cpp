#include <stdio.h>
#include <signal.h>
#include <unistd.h>

struct AlarmHandler
{
    static void handle(int signum)
    {
        printf("Alarm: %s\n", message);
        alarm(1);
    }
    static const char* message;
};
const char* AlarmHandler::message = NULL;

int main()
{
    AlarmHandler::message = "message from AlarmHandler";
    signal(SIGALRM, AlarmHandler::handle);
    alarm(1);
    getchar();   
    return 0;
}
