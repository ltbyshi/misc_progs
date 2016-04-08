#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

FILE *fopen(const char *path, const char *mode)
{
    printf("fopen(%s, %s) => NULL\n", path, mode);
    return NULL;
}

FILE *fopen64(const char *path, const char *mode)
{
    printf("fopen64(%s, %s) => NULL\n", path, mode);
    return NULL;
}

FILE *_IO_new_fopen(const char *path, const char *mode)
{
    printf("_IO_new_fopen(%s, %s) => NULL\n", path, mode);
    return NULL;
}

int open(const char *pathname, int flags, ...)
{
    printf("open(%s, %d) => NULL\n", pathname, flags);
    return -1;
}

int open64(const char *pathname, int flags, ...)
{
    printf("open64(%s, %d) => NULL\n", pathname, flags);
    return -1;
}

FILE *fdopen64(int fd, const char *mode)
{
    printf("fdopen64(%d, %s) => NULL\n", fd, mode);
    return NULL;
}

FILE *freopen64(const char *path, const char *mode, FILE *stream)
{
    printf("freopen64(%s, %s) => NULL\n", path, mode);
    return NULL;
}

DIR *opendir(const char *name)
{
    printf("opendir(%s) => NULL\n", name);
    return NULL;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) { return 0; }
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) { return 0; }

