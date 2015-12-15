#include <stdlib.h>

void InvalidPointer()
{
    char* p;
    p = (char*)malloc(1024);
    p = p - 1;
    free(p);
}

void DoubleFree()
{
    char* p;
    p = (char*)malloc(1);
    free(p);
    free(p);
}

int main()
{
    DoubleFree();
    return 0;
}
