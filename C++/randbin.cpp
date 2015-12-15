#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define RANDBIN(typestr,type,size,data) \
    else if(!strcmp(#type, typestr)){type* p; size_t i; \
    typesize = sizeof(type); \
    data = (type*)malloc(sizeof(type)*size); p = (type*)data; \
    for(i = 0; i < size; i ++) p[i] = (type)rand(); }

void RandBin(const char* type, size_t size, const char* filename)
{
    void* data;
    uint64_t typesize;
    uint64_t nelems;
    FILE* fp;
    fp = fopen(filename, "wb");
    if(!fp)
    {
        perror("Error: cannot open output file");
        exit(1);
    }
    if(0) {}
    RANDBIN(type, char, size, data)
    RANDBIN(type, short, size, data)
    RANDBIN(type, int, size, data)
    RANDBIN(type, long int, size, data)
    RANDBIN(type, float, size, data)
    RANDBIN(type, double, size, data)
    else
    {
        fprintf(stderr, "Unsupported type: %s\n", type);
        exit(1);
    }
    nelems = size;
    fwrite(&typesize, sizeof(typesize), 1, fp);
    fwrite(&nelems, sizeof(nelems), 1, fp);
    fwrite(data, size*typesize, 1, fp);
    fclose(fp);
    free(data);
}

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        fprintf(stderr, "Usage: %s type size filename\n", argv[0]);
        exit(1);
    }
    srand(time(0));
    RandBin(argv[1], strtoul(argv[2], NULL, 10), argv[3]);
    return 0;
}
