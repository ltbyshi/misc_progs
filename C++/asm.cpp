#include <stdio.h>

const char* msg = "Hello world!";
int main()
{
    while(true)
    {
        asm("sysenter");
        //asm("leave");
        //asm("call main");
        //asm("leave");
        //asm("ret");
    }
    asm("cli");
    return 0;
}
