#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cerr << "Error: invalid argument" << endl;
        cerr << "Usage: mmap filename" << endl;
        exit(1);
    }
    const char* filename = argv[1];
    int prot = PROT_READ;
    int flags = MAP_SHARED;
    
    FILE* fp = fopen(filename, "rb");
    if(!fp)
    {
        perror("Error in fopen: ");
        exit(1);
    }
    int fd = fileno(fp);
    struct stat st;
    if(fstat(fd, &st) == -1)
    {
        perror("Error in fstat: ");
        exit(1);
    }
    
    size_t length = st.st_size;
    long pagesize = sysconf(_SC_PAGESIZE);
    
    void* mem = mmap(NULL, length, prot, flags, fd, 0);
    if(mem == MAP_FAILED)
    {
        perror("Error in mmap: ");
        exit(1);
    }
    // read
    unsigned long sum = 0;
    char* p = (char*)mem;
    for(size_t i = 0; i < length; i += pagesize)
        sum += p[i];
    cout << "sum of bytes: " << sum << endl;
    //fwrite(mem, length, 1, stdout);
    /* write
    srand((unsigned int)time(0));
    char* p = (char*)mem;
    for(int i = 0; i < 1000000; i ++)
    {
        p[rand() % length] = 77;
    }
    munmap(mem, length);
    */
    return 0;
}
