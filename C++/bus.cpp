#include <stdio.h>
#include <stdlib.h>

int main()
{
    __asm__("pushf\norl $0x40000,(%rsp)\npopf");
    int* pi;
    char* pc;
    pc = (char*)malloc(1024+1);
    pc ++;
    pi = (int*)pc;
    *pi = 12;

    printf("%d\n", *(pi++));
    return 0;
}

