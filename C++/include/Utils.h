#ifndef __UTILS_H__
#define __UTILS_H__
#include <sstream>
#include <string>
#include <map>
#include "ProgressBar.h"
#include "BitSet.h"
#include "Exception.h"
#include "IOUtils.h"
#include "Statistics.h"
#include "Logging.h"
#include "Exception.h"

void Die(const char* format, ...);

// Null reference
template <typename Type>
inline Type& NullRef()
{
    return *((Type*)0);
}

// command line parsing
typedef int (*CommandHandler)(int , char**);
typedef std::map<std::string, CommandHandler> HandlerMap;
void ShowUsage(const HandlerMap& handlers);
int ParseArguments(const HandlerMap& handlers, const char* progname,
                    int argc, char** argv);


inline std::string ToBinaryString(unsigned int a)
{
    std::string s(32 + 3, '0');
    
    for(int i = 0, si = 0; i < 32; i ++, si ++)
    {
        if(a & 0x80000000)
            s[si] = '1';
        a <<= 1;
        if((i % 8 == 7) && (i != 31))
        {
            si ++;
            s[si] = ' ';
        }
    }   
    return s;
}

inline std::vector<std::string>& Split(const std::string& s,
        std::vector<std::string>& v,
        char delimiter='\t')
{
    std::istringstream is(s);
    std::string tok;
    while(std::getline(is, tok, delimiter))
        v.push_back(tok);
    return v;
}

inline std::vector<std::string> Split(const std::string& s,
        char delimiter='\t')
{
    std::istringstream is(s);
    std::string tok;
    std::vector<std::string> v;
    while(std::getline(is, tok, delimiter))
        v.push_back(tok);
    return v;
}

// terminal



#endif
