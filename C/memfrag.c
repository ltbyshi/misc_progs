#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#define POOLSIZE (1UL << 16)

int main()
{
    char** pool;
    size_t i;

    pool = (char**)malloc(POOLSIZE*sizeof(int*));
    for(i = 0; i < POOLSIZE; i ++)
        pool[i] = (char*)malloc(sizeof(char));
    printf("Allocated %lu blocks\n", POOLSIZE);
    malloc_stats();
    sleep(8);
    for(i = 0; i < POOLSIZE; i += 2)
        free(pool[i]);
    printf("Free\n");
    malloc_stats();
    sleep(8);
    free(pool);
    return 0;
}
