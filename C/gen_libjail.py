#! /usr/bin/env python
import json, string

prelogue = '''
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
            fprintf(stderr, "Error: $LIBJAIL_ROOT is not defined\\n");
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
        debug("%s is jailed in %s()\\n", path, funcname);
        return 0;
    }
    return 1;
}
'''

body = string.Template('''/* ${name} */
typedef ${ret_type} (*REAL_${name})(${arg_decl});
static REAL_${name} real_${name} = NULL;

${ret_type} ${name}(${arg_decl})
{
    debug("${name}('%s')\\n", ${arg_path});
    if(!real_${name})
        real_${name} = (REAL_${name})dlsym(RTLD_NEXT, "${name}");
    if(!check_path(${arg_path}, "${name}"))
    {
        errno = EACCES;
        return ${ret_val};
    }
    return real_${name}(${arg_names});
}
''')

body_open = string.Template('''/* ${name} */
typedef ${ret_type} (*REAL_${name})(${arg_decl}, ...);
static REAL_${name} real_${name} = NULL;

${ret_type} ${name}(${arg_decl}, ...)
{
    mode_t mode;
    va_list arg;
    va_start(arg, flags);

    debug("${name}('%s', '%d')\\n", ${arg_path}, flags);
    if(!real_${name})
        real_${name} = (REAL_${name})dlsym(RTLD_NEXT, "${name}");
    if(flags & O_CREAT)
    {
        mode = va_arg(arg, int);
        va_end(arg);
    }
    if(((flags & O_CREAT) & ((mode & O_RDWR) || (mode & O_WRONLY))) ||
       (flags & O_TRUNC) || (flags & O_APPEND))
    {
        if(!check_path(${arg_path}, "${name}"))
        {
            errno = EACCES;
            return ${ret_val};
        }
    }
    return real_${name}(${arg_names}, mode);
}
''')


body_fopen = string.Template('''/* ${name} */
typedef ${ret_type} (*REAL_${name})(${arg_decl});
static REAL_${name} real_${name} = NULL;

${ret_type} ${name}(${arg_decl})
{
    debug("${name}('%s', '%s')\\n", ${arg_path}, mode);
    if(!real_${name})
        real_${name} = (REAL_${name})dlsym(RTLD_NEXT, "${name}");
    if((strstr(mode, "w") != NULL) ||
       (strstr(mode, "a") != NULL) ||
       (strstr(mode, "+") != NULL))
    {
        if(!check_path(${arg_path}, "${name}"))
        {
            errno = EACCES;
            return ${ret_val};
        }
    }
    return real_${name}(${arg_names});
}
''')

if __name__ == '__main__':
    defs = json.load(open('libjail_def.json', 'r'))
    print prelogue
    for name in defs:
        d = defs[name]
        arg_names = ', '.join([a[1] for a in d['args']])
        arg_decl = ', '.join([a[0] + ' ' + a[1] for a in d['args']])
        t = body
        if name in ('open', 'open64', 'openat', 'openat64'):
            t = body_open
        elif name in ('fopen', 'fopen64', 'freopen', 'freopen64'):
            t = body_fopen
        print t.substitute(name=name,
                ret_type=d['ret_type'],
                arg_names=arg_names,
                arg_decl=arg_decl,
                arg_path=d['arg_path'],
                ret_val=d['ret_val'])

