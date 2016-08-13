#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Utils.h"

void Die(const char* format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    std::cerr << "Error: " << buffer << std::endl;
    exit(-1);
}


// command line parsing
void ShowUsage(const HandlerMap& handlers, const char* progname)
{
    cerr << "Usage: " << progname << " command [options]" << endl;
    cerr << "Avaiable commands:";
    for(HandlerMap::const_iterator it = handlers.begin();
        it != handlers.end(); ++ it)
        cerr << " " << it->first;
    cerr << endl;
    exit(1);
}

int ParseArguments(const HandlerMap& handlers, const char* progname,
                    int argc, char** argv)
{
    if(argc < 2)
    {
        ShowUsage(handlers, progname);
    }
    HandlerMap::const_iterator it = handlers.find(argv[1]);
    if(it != handlers.end())
        return it->second(argc - 1, argv + 1);
    else
    {
        cerr << "Error: invalid command: " << argv[1] << "\n" << endl;
        ShowUsage(handlers, progname);
        return 1;
    }
}



