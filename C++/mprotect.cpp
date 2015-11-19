#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    long pagesize = sysconf(_SC_PAGE_SIZE);
    printf("pagesize: %d\n", pagesize);
    if(mprotect((void*)main, pagesize, PROT_NONE) != 0)
        perror("mprotect");
    *((int*)main) = 0;
    return 0;
}
