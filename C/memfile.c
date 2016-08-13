#include <stdio.h>
#include <stdlib.h>

const size_t filesize = 1024;

int main(int argc, char** argv)
{
    FILE* fp;
    char* buf;
    int data = 7;
    buf = (char*)malloc(filesize);
    fp = fmemopen(buf, filesize, "wb+");
    while(1)
    {
        if(fwrite(&data, sizeof(data), 1, fp) < 1)
            break;
    }
    fclose(fp);
    return 0;
}
