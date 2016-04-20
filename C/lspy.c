#include <stdio.h>
#include <stdlib.h>
/*#define _GNU_SOURCE*/
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <libgen.h>
#include <limits.h>

/*
static int MatchPath(const char* path)
{
    char *pattern;
    char *dirc, *basec, *dname, *bname;
    char patc[PATH_MAX];
    char cwd[PATH_MAX];
    char pathc[PATH_MAX];
    int found;

    pattern = getenv("LSPY_PATTERN");
    if(!pattern)
        return 1;
    getcwd(cwd, PATH_MAX);
    strcpy(patc, pattern);
    dirc = strdup(path);
    basec = strdup(path);
    dname = dirname(dirc);
    bname = basename(basec);
    if(patc[0] != '/')
    {
       strcpy(patc, cwd);   
       strcat(patc, "/");
       strcat(patc, pattern);
    }
    if(dirc[0] != '/')
    {
        strcpy(pathc, cwd);
        if(dirc[0] != '.')
        {
            strcat(pathc, "/");
            strcat(pathc, dname);
        }
        strcat(pathc, "/");
        if(bname[0] != '.')
            strcat(pathc, bname);
    }
    else
        strcpy(pathc, path);

    if(strstr(pathc, patc))
        found = 1;
    else
        found = 0;
    free(dirc);
    free(basec);
    return found;
}
*/

static int MatchPath(const char* path)
{
    char *pattern;
    char fullpath[PATH_MAX];
    int found;
    pattern = getenv("LSPY_PATTERN");
    if(!pattern)
        return 1;

    realpath(path, fullpath);
    if(strstr(path, pattern))
        found = 1;
    else
        found = 0;
    return found;
}

typedef int (*REAL_open)(const char *path, int flags, mode_t mode);
static REAL_open real_open = NULL;

int open(const char *path, int flags, mode_t mode)
{
    if(!real_open)
        real_open = (REAL_open)dlsym(RTLD_NEXT, "open");
    if(MatchPath(path))
    {
        char fullpath[PATH_MAX];
        realpath(path, fullpath);
        fprintf(stderr, "[lspy] open(%s, %d, 0%o)\n", fullpath, flags, mode);
    }
    return real_open(path, flags, mode);
}


typedef FILE* (*REAL_fopen)(const char *path, const char *mode);
static REAL_fopen real_fopen = NULL;

FILE *fopen(const char *path, const char *mode)
{
    if(!real_fopen)
        real_fopen = (REAL_fopen)dlsym(RTLD_NEXT, "fopen");
    if(MatchPath(path))
    {
        char fullpath[PATH_MAX];
        realpath(path, fullpath);
        fprintf(stderr, "[lspy] fopen(%s, %s)\n", fullpath, mode);
    }
    return real_fopen(path, mode);
}

typedef FILE* (*REAL_fopen64)(const char *path, const char *mode);
static REAL_fopen64 real_fopen64 = NULL;

FILE *fopen64(const char *path, const char *mode)
{
    if(!real_fopen64)
        real_fopen64 = (REAL_fopen64)dlsym(RTLD_NEXT, "fopen64");
    if(!real_fopen64)
    {
        fprintf(stderr, "[lspy] Error: cannot resolve symbol %s\n", "fopen64");
        abort();
    }
    
    if(MatchPath(path))
    {
        char fullpath[PATH_MAX];
        realpath(path, fullpath);
        fprintf(stderr, "[lspy] fopen64(%s, %s)\n", fullpath, mode);
    }
    return real_fopen64(path, mode);
}

#include <dirent.h>
typedef struct dirent* (*REAL_readdir)(DIR* dirp);
static REAL_readdir real_readdir = NULL;

struct dirent* readdir(DIR* dirp)
{
    if(!real_readdir)
        real_readdir = (REAL_readdir)dlsym(RTLD_NEXT, "readdir");
    if(!real_readdir)
    {
        fprintf(stderr, "[lspy] Error: cannot resolve symbol %s\n", "readdir");
        abort();
    }
    
    fprintf(stderr, "[lspy] readdir(%p)\n", dirp);
    return real_readdir(dirp);
}

typedef DIR* (*REAL_opendir)(const char* name);
static REAL_opendir real_opendir = NULL;

DIR* opendir(const char* name)
{
    if(!real_opendir)
        real_opendir = (REAL_opendir)dlsym(RTLD_NEXT, "opendir");
    if(!real_opendir)
    {
        fprintf(stderr, "[lspy] Error: cannot resolve symbol %s\n", "opendir");
        abort();
    }
    
    fprintf(stderr, "[lspy] opendir('%s')\n", name);
    return real_opendir(name);
}

typedef int (*REAL_access)(const char *path, int amode);
static REAL_access real_access = NULL;

int access(const char *path, int amode)
{
    if(!real_access)
        real_access = (REAL_access)dlsym(RTLD_NEXT, "access");
    if(!real_access)
    {
        fprintf(stderr, "[lspy] Error: cannot resolve symbol %s\n", "access");
        abort();
    }
    
    fprintf(stderr, "[lspy] access('%s', %d)\n", path, amode);
    return real_access(path, amode);
}

#include <sys/stat.h>
typedef int (*REAL_stat)(const char*  path, struct stat* buf);
static REAL_stat real_stat = NULL;

int stat(const char *path, struct stat *buf)
{
    if(!real_stat)
        real_stat = (REAL_stat)dlsym(RTLD_NEXT, "stat");
    if(!real_stat)
    {
        fprintf(stderr, "[lspy] Error: cannot resolve symbol %s\n", "stat");
        abort();
    }
    
    fprintf(stderr, "[lspy] stat('%s', %p)\n", path, buf);
    return real_stat(path, buf);
}


#if ENABLE_MPI
#include <mpi.h>
typedef int (*REAL_MPI_Init)(int *argc, char ***argv);
static REAL_MPI_Init real_MPI_Init = NULL;

int MPI_Init(int *argc, char ***argv)
{
    if(!real_MPI_Init)
        real_MPI_Init = (REAL_MPI_Init)dlsym(RTLD_NEXT, "MPI_Init");
    if(!real_MPI_Init)
    {
        fprintf(stderr, "[lspy] Error: cannot resolve symbol %s\n", "MPI_Init");
        abort();
    }
           
    printf("[lspy] MPI_Init(%p, %p)\n", argc, argv);
    return real_MPI_Init(argc, argv);
}

#include <sys/types.h>
#include <sys/socket.h>

typedef int (*REAL_connect)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
static REAL_connect real_connect = NULL;

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    if(!real_connect)
        real_connect = (REAL_connect)dlsym(RTLD_NEXT, "connect");
    if(!real_connect)
    {
        fprintf(stderr, "[lspy] Error: cannot resolve symbol %s\n", "connect");
        abort();
    }

    printf("[lspy] connect(%d, (%u, %s), %d)\n", sockfd, addr->sa_family, addr->sa_data, addrlen);
    return real_connect(sockfd, addr, addrlen);
}
#endif
