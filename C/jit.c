#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

int Add(int a, int b)
{
    return a + b;
}

void Print(const char* s)
{
    printf("%s\n", s);
}

void ENDF()
{
    printf("End of function");
}

typedef int (*FN_ADD)(int, int);
typedef void (*FN_PRINT)(const char* s);

void PrintBinary(void* data, size_t size)
{
    size_t i;
    for(i = 0;i < size; i ++)
        printf("%02x ", (unsigned int)*((unsigned char*)data + i));
    printf("\n");
}

void* MapFunction(void* start, void* end)
{
    char* mem;
    size_t pagesize, length, memsize;

    pagesize = sysconf(_SC_PAGESIZE);
    length = ((char*)end - (char*)start);
    memsize = (length / pagesize + 1)*pagesize;

    mem = (char*)mmap(NULL, memsize, PROT_WRITE|PROT_READ, 
            MAP_ANON|MAP_PRIVATE, -1, 0);
    if(mem == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }
    memcpy(mem, start, length);
    if(mprotect(mem, length, PROT_EXEC|PROT_READ) == -1)
    {
        perror("mprotect");
        exit(-1);
    }

    printf("MapFunction: %p - %p (%lu bytes)\n", start, end, length);
    PrintBinary(mem, length);
    return (void*)mem;
}

int main(int argc, char** argv)
{
    FN_ADD fn_add;
    FN_PRINT fn_print;
    int result;

    fn_add = (FN_ADD)MapFunction(Add, Print);
    result = fn_add(3, 4);
    printf("3 + 4 = %d\n", result);

    fn_print = (FN_PRINT)MapFunction(Print, ENDF);
    fn_print("Hello world!\n");
    fn_print("FNPRINT\n");
    fn_print("MapFunction\n");

    return 0;
}


