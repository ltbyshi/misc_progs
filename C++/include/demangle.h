#ifndef __DEMANGLE_H__
#define __DEMANGLE_H__
#include <string>
#include <cxxabi.h>

inline std::string demangle(const std::string& name)
{
    int status;
    char* s = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
    std::string fullname(s);
    free(s);
    return fullname;
}

#endif

