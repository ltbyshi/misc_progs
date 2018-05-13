#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>

static char jail_root[PATH_MAX] = {0};
static int jail_debug = -1;

static void debug(const char* format, ...)
{
    if(jail_debug == -1)
    {
        char* e;
        e = getenv("LIBJAIL_DEBUG");
        if(e && atoi(e))
            jail_debug = 1;
        else
            jail_debug = 0;
    }
    if(jail_debug)
    {
        va_list args;
        va_start(args, format);
        fprintf(stderr, "[libjail] ");
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

static int check_path(const char* path, const char* funcname)
{
    char resolved_path[PATH_MAX];
    char rpath_b[PATH_MAX];
    char* rpath;
    struct stat st;

    if(!jail_root[0])
    {
        char* e;
        e = getenv("LIBJAIL_ROOT"); 
        if(!e)
        {
            fprintf(stderr, "Error: $LIBJAIL_ROOT is not defined\n");
            exit(10);
        }
        realpath(e, jail_root);
    }
    /* resolve realpath */
    strcpy(rpath_b, path);
    rpath = rpath_b;
    while(realpath(rpath, resolved_path) == NULL)
    {
        if(!strcmp(rpath, "/") || !strcmp(rpath, "."))
        {
            strcpy(resolved_path, rpath);
            break;
        }
        rpath = dirname(rpath);
    }
    /* ignore files that are not owned by the effective user */
    if(stat(resolved_path, &st) != -1)
    {
        if(st.st_uid != geteuid())
            return 1;
    }
    /* ignore special directories */
    if((strstr(resolved_path, "/proc") == resolved_path) ||
       (strstr(resolved_path, "/dev") == resolved_path))
       return 1;
    /* compare path prefix to jail root */
    if(strstr(resolved_path, jail_root) != resolved_path)
    {
        debug("%s is jailed in %s()\n", path, funcname);
        return 0;
    }
    return 1;
}

/* rename */
typedef int (*REAL_rename)(const char* oldpath, const char* newpath);
static REAL_rename real_rename = NULL;

int rename(const char* oldpath, const char* newpath)
{
    debug("rename('%s')\n", oldpath);
    if(!real_rename)
        real_rename = (REAL_rename)dlsym(RTLD_NEXT, "rename");
    if(!check_path(oldpath, "rename"))
    {
        errno = EACCES;
        return -1;
    }
    return real_rename(oldpath, newpath);
}

/* unlink */
typedef int (*REAL_unlink)(const char* path);
static REAL_unlink real_unlink = NULL;

int unlink(const char* path)
{
    debug("unlink('%s')\n", path);
    if(!real_unlink)
        real_unlink = (REAL_unlink)dlsym(RTLD_NEXT, "unlink");
    if(!check_path(path, "unlink"))
    {
        errno = EACCES;
        return -1;
    }
    return real_unlink(path);
}

/* renameat */
typedef int (*REAL_renameat)(int oldfd, const char* oldpath, int newfd, const char* newpath);
static REAL_renameat real_renameat = NULL;

int renameat(int oldfd, const char* oldpath, int newfd, const char* newpath)
{
    debug("renameat('%s')\n", oldpath);
    if(!real_renameat)
        real_renameat = (REAL_renameat)dlsym(RTLD_NEXT, "renameat");
    if(!check_path(oldpath, "renameat"))
    {
        errno = EACCES;
        return -1;
    }
    return real_renameat(oldfd, oldpath, newfd, newpath);
}

/* creat */
typedef int (*REAL_creat)(const char* path, mode_t mode);
static REAL_creat real_creat = NULL;

int creat(const char* path, mode_t mode)
{
    debug("creat('%s')\n", path);
    if(!real_creat)
        real_creat = (REAL_creat)dlsym(RTLD_NEXT, "creat");
    if(!check_path(path, "creat"))
    {
        errno = EACCES;
        return -1;
    }
    return real_creat(path, mode);
}

/* open64 */
typedef int (*REAL_open64)(const char* path, int flags, ...);
static REAL_open64 real_open64 = NULL;

int open64(const char* path, int flags, ...)
{
    mode_t mode;
    va_list arg;
    va_start(arg, flags);

    debug("open64('%s', '%d')\n", path, flags);
    if(!real_open64)
        real_open64 = (REAL_open64)dlsym(RTLD_NEXT, "open64");
    if(flags & O_CREAT)
    {
        mode = va_arg(arg, int);
        va_end(arg);
    }
    if(((flags & O_CREAT) & ((mode & O_RDWR) || (mode & O_WRONLY))) ||
       (flags & O_TRUNC) || (flags & O_APPEND))
    {
        if(!check_path(path, "open64"))
        {
            errno = EACCES;
            return -1;
        }
    }
    return real_open64(path, flags, mode);
}

/* openat */
typedef int (*REAL_openat)(int dirfd, const char* path, int flags, ...);
static REAL_openat real_openat = NULL;

int openat(int dirfd, const char* path, int flags, ...)
{
    mode_t mode;
    va_list arg;
    va_start(arg, flags);

    debug("openat('%s', '%d')\n", path, flags);
    if(!real_openat)
        real_openat = (REAL_openat)dlsym(RTLD_NEXT, "openat");
    if(flags & O_CREAT)
    {
        mode = va_arg(arg, int);
        va_end(arg);
    }
    if(((flags & O_CREAT) & ((mode & O_RDWR) || (mode & O_WRONLY))) ||
       (flags & O_TRUNC) || (flags & O_APPEND))
    {
        if(!check_path(path, "openat"))
        {
            errno = EACCES;
            return -1;
        }
    }
    return real_openat(dirfd, path, flags, mode);
}

/* mkdir */
typedef int (*REAL_mkdir)(const char* path, mode_t mode);
static REAL_mkdir real_mkdir = NULL;

int mkdir(const char* path, mode_t mode)
{
    debug("mkdir('%s')\n", path);
    if(!real_mkdir)
        real_mkdir = (REAL_mkdir)dlsym(RTLD_NEXT, "mkdir");
    if(!check_path(path, "mkdir"))
    {
        errno = EACCES;
        return -1;
    }
    return real_mkdir(path, mode);
}

/* openat64 */
typedef int (*REAL_openat64)(int dirfd, const char* path, int flags, ...);
static REAL_openat64 real_openat64 = NULL;

int openat64(int dirfd, const char* path, int flags, ...)
{
    mode_t mode;
    va_list arg;
    va_start(arg, flags);

    debug("openat64('%s', '%d')\n", path, flags);
    if(!real_openat64)
        real_openat64 = (REAL_openat64)dlsym(RTLD_NEXT, "openat64");
    if(flags & O_CREAT)
    {
        mode = va_arg(arg, int);
        va_end(arg);
    }
    if(((flags & O_CREAT) & ((mode & O_RDWR) || (mode & O_WRONLY))) ||
       (flags & O_TRUNC) || (flags & O_APPEND))
    {
        if(!check_path(path, "openat64"))
        {
            errno = EACCES;
            return -1;
        }
    }
    return real_openat64(dirfd, path, flags, mode);
}

/* remove */
typedef int (*REAL_remove)(const char* path);
static REAL_remove real_remove = NULL;

int remove(const char* path)
{
    debug("remove('%s')\n", path);
    if(!real_remove)
        real_remove = (REAL_remove)dlsym(RTLD_NEXT, "remove");
    if(!check_path(path, "remove"))
    {
        errno = EACCES;
        return -1;
    }
    return real_remove(path);
}

/* freopen64 */
typedef FILE* (*REAL_freopen64)(const char* path, const char* mode, FILE* stream);
static REAL_freopen64 real_freopen64 = NULL;

FILE* freopen64(const char* path, const char* mode, FILE* stream)
{
    debug("freopen64('%s', '%s')\n", path, mode);
    if(!real_freopen64)
        real_freopen64 = (REAL_freopen64)dlsym(RTLD_NEXT, "freopen64");
    if((strstr(mode, "w") != NULL) ||
       (strstr(mode, "a") != NULL) ||
       (strstr(mode, "+") != NULL))
    {
        if(!check_path(path, "freopen64"))
        {
            errno = EACCES;
            return NULL;
        }
    }
    return real_freopen64(path, mode, stream);
}

/* rmdir */
typedef int (*REAL_rmdir)(const char* path);
static REAL_rmdir real_rmdir = NULL;

int rmdir(const char* path)
{
    debug("rmdir('%s')\n", path);
    if(!real_rmdir)
        real_rmdir = (REAL_rmdir)dlsym(RTLD_NEXT, "rmdir");
    if(!check_path(path, "rmdir"))
    {
        errno = EACCES;
        return -1;
    }
    return real_rmdir(path);
}

/* freopen */
typedef FILE* (*REAL_freopen)(const char* path, const char* mode, FILE* stream);
static REAL_freopen real_freopen = NULL;

FILE* freopen(const char* path, const char* mode, FILE* stream)
{
    debug("freopen('%s', '%s')\n", path, mode);
    if(!real_freopen)
        real_freopen = (REAL_freopen)dlsym(RTLD_NEXT, "freopen");
    if((strstr(mode, "w") != NULL) ||
       (strstr(mode, "a") != NULL) ||
       (strstr(mode, "+") != NULL))
    {
        if(!check_path(path, "freopen"))
        {
            errno = EACCES;
            return NULL;
        }
    }
    return real_freopen(path, mode, stream);
}

/* fopen64 */
typedef FILE* (*REAL_fopen64)(const char* path, const char* mode);
static REAL_fopen64 real_fopen64 = NULL;

FILE* fopen64(const char* path, const char* mode)
{
    debug("fopen64('%s', '%s')\n", path, mode);
    if(!real_fopen64)
        real_fopen64 = (REAL_fopen64)dlsym(RTLD_NEXT, "fopen64");
    if((strstr(mode, "w") != NULL) ||
       (strstr(mode, "a") != NULL) ||
       (strstr(mode, "+") != NULL))
    {
        if(!check_path(path, "fopen64"))
        {
            errno = EACCES;
            return NULL;
        }
    }
    return real_fopen64(path, mode);
}

/* truncate */
typedef int (*REAL_truncate)(const char* path, off_t length);
static REAL_truncate real_truncate = NULL;

int truncate(const char* path, off_t length)
{
    debug("truncate('%s')\n", path);
    if(!real_truncate)
        real_truncate = (REAL_truncate)dlsym(RTLD_NEXT, "truncate");
    if(!check_path(path, "truncate"))
    {
        errno = EACCES;
        return -1;
    }
    return real_truncate(path, length);
}

/* fopen */
typedef FILE* (*REAL_fopen)(const char* path, const char* mode);
static REAL_fopen real_fopen = NULL;

FILE* fopen(const char* path, const char* mode)
{
    debug("fopen('%s', '%s')\n", path, mode);
    if(!real_fopen)
        real_fopen = (REAL_fopen)dlsym(RTLD_NEXT, "fopen");
    if((strstr(mode, "w") != NULL) ||
       (strstr(mode, "a") != NULL) ||
       (strstr(mode, "+") != NULL))
    {
        if(!check_path(path, "fopen"))
        {
            errno = EACCES;
            return NULL;
        }
    }
    return real_fopen(path, mode);
}

/* open */
typedef int (*REAL_open)(const char* path, int flags, ...);
static REAL_open real_open = NULL;

int open(const char* path, int flags, ...)
{
    mode_t mode;
    va_list arg;
    va_start(arg, flags);

    debug("open('%s', '%d')\n", path, flags);
    if(!real_open)
        real_open = (REAL_open)dlsym(RTLD_NEXT, "open");
    if(flags & O_CREAT)
    {
        mode = va_arg(arg, int);
        va_end(arg);
    }
    if(((flags & O_CREAT) & ((mode & O_RDWR) || (mode & O_WRONLY))) ||
       (flags & O_TRUNC) || (flags & O_APPEND))
    {
        if(!check_path(path, "open"))
        {
            errno = EACCES;
            return -1;
        }
    }
    return real_open(path, flags, mode);
}

/* mkdirat */
typedef int (*REAL_mkdirat)(int fd, const char* path, mode_t mode);
static REAL_mkdirat real_mkdirat = NULL;

int mkdirat(int fd, const char* path, mode_t mode)
{
    debug("mkdirat('%s')\n", path);
    if(!real_mkdirat)
        real_mkdirat = (REAL_mkdirat)dlsym(RTLD_NEXT, "mkdirat");
    if(!check_path(path, "mkdirat"))
    {
        errno = EACCES;
        return -1;
    }
    return real_mkdirat(fd, path, mode);
}

/* unlinkat */
typedef int (*REAL_unlinkat)(int fd, const char* path, int flag);
static REAL_unlinkat real_unlinkat = NULL;

int unlinkat(int fd, const char* path, int flag)
{
    debug("unlinkat('%s')\n", path);
    if(!real_unlinkat)
        real_unlinkat = (REAL_unlinkat)dlsym(RTLD_NEXT, "unlinkat");
    if(!check_path(path, "unlinkat"))
    {
        errno = EACCES;
        return -1;
    }
    return real_unlinkat(fd, path, flag);
}

