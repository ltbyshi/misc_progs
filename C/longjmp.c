#include <setjmp.h>
#include <stdio.h>

jmp_buf jmp_foo, jmp_bar;
jmp_buf jmp_range;

void bar()
{
    setjmp(jmp_bar);
    longjmp(jmp_foo, 1);
    printf("longjmp\n");
}

void foo()
{
    int junk[100];
    setjmp(jmp_foo);
    longjmp(jmp_bar, 1);
    printf("setjmp %d\n", junk[8]);
}


int xrange(int n)
{
    int i;
    if(setjmp(jmp_range) == 0)
        return 0;
    for(i = 0; i < n; i ++)
    {
        if(setjmp(jmp_range) == 0)
            return i;
    }
    return -1;
}

int main()
{
    xrange(5);
    while(1)
    {
        int i;
        if(i != -1)
        {
           longjmp(jmp_range, 1);
           printf("%d ", i);
 
        }
        else
            break;
    }
    printf("\n");
    return 0;
}
