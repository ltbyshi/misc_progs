#include <stdio.h>

const char* msg = "Hello world!";
typedef void (*FPTR)(void);
FPTR p = (FPTR)0x7ffff7df4843;

int main()
{
   for(int i = 0; i < 1000000; i ++)
    {
        asm("mov %0,%%eax\n"
                "syscall\n"
                "cmp $0xfffffffffffff001,%%rax\n"
                : 
                : "r"(i), "r"(p)
                :);
    }
    // asm("cli");
    return 0;
}
