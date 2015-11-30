#include <unistd.h>

int main()
{
    char* p;

    p = (char*)main;
    while(1)
    {
        sbrk(1024*1024);
    }
        // brk(p++);
    return 0;
}
