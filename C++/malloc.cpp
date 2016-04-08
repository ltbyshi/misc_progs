#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv)
{
    long size = 0;
    char* endptr = NULL;
    char* mem = NULL;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s size\n", argv[0]);
        exit(1);
    }
    size = strtol(argv[1], &endptr, 10);
    if(errno == EINVAL)
    {
        fprintf(stderr, "Invalid number\n");
        exit(1);
    }
    if(errno == ERANGE)
    {
        fprintf(stderr, "Size out of range\n");
        exit(1);
    }
    if(endptr == argv[1])
    {
        fprintf(stderr, "No size given\n");
        exit(1);
    }
    switch(*endptr)
    {
        case 'K':
        case 'k':
            size *= (1UL << 10);
            break;
        case 'M':
        case 'm':
            size *= (1UL << 20);
            break;
        case 'g':
        case 'G':
            size *= (1UL << 30);
            break;
        case 't':
        case 'T':
            size *= (1UL << 40);
    }
    printf("malloc %ld bytes\n", size);
    mem = (char*)malloc(size);
    if(!mem)
    {
        perror("malloc() failed");
        exit(1);
    }
    printf("memset\n");
    memset(mem, 0, size);
    free(mem);
    
    return 0;
}
