#include <iostream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

#include <unistd.h>
#include <getopt.h>


void PrintOption(option* opt)
{
    switch(opt->has_arg)
    {   
        case no_argument:
            cout << "Flag\t" << opt->name << "\tTrue" << "\t" << optind << endl;
            break;
        case required_argument:
            cout << "Required\t" << opt->name << "\t" << optarg << "\t" << optind << endl;
            break;
        case optional_argument:
            cout << "Optional\t" << opt->name << "\t" << optarg << "\t" << optind << endl;
            break;
        default:
            cerr << "Error: invalid has_arg value" << endl;
    }
}

int main(int argc, char** argv)
{
    static option longopts[] = 
    {
        {"verbose",     no_argument,       NULL, 0},
        {"number",      required_argument, NULL, 0},
        {"min-length",  required_argument, NULL, 0},
        {"max-length",  required_argument, NULL, 0},
        {"length",      required_argument, NULL, 0},
        {"alphabet",    required_argument, NULL, 0},
        {"format",      required_argument, NULL, 0},
        {"prefix",      required_argument, NULL, 0},
        {"outfile",     required_argument, NULL, 0},
        {0,             0,                 0,    0}
    };
    static char shortopts[] = "vn--lafpo";
    // generate option string
    int numopts = sizeof(longopts)/sizeof(option) - 1;
    ostringstream os;
    for(int i = 0; i < numopts; i ++)
    {
        if(shortopts[i] == '-')
            continue;
        switch(longopts[i].has_arg)
        {
            case no_argument:
                os << shortopts[i];
                break;
            case required_argument:
                os << shortopts[i] << ":";
                break;
            case optional_argument:
                os << shortopts[i] << "::";
                break;
        }
    }
    string optstring = os.str();
    cerr << "optstring: " << optstring << endl;
    bool* optset = new bool[numopts];
    for(int i = 0; i < numopts; i ++)
        optset[i] = false;
    
    // set the id of the options
    for(int i = 0; i < numopts; i ++)
        longopts[i].val = 1000;
    int option_index = 0;
    int c = 0;
    bool stop = false;
    // opterr = 0;
    while((c = getopt_long(argc, argv, 
        optstring.c_str(), longopts, &option_index)) != -1)
    {
        if(c == '?')
        {
            //cerr << "Error: unknown argument: " << optarg << endl;
            break;
        }
        // short option
        else if(c < 1000)
        {
            for(int i = 0; i < numopts; i ++)
            {
                if(c == shortopts[i])
                {
                    if(optset[i])
                    {
                        cerr << "Error: duplicated option: " << longopts[i].name << endl;
                        stop = true;
                        break;
                    }
                    PrintOption(&longopts[i]);
                    optset[i] = true;
                    break;
                }
            }
        }
        else
        {
            if(optset[option_index])
            {
                cerr << "Error: duplicate option: " << longopts[option_index].name << endl;
                stop = true;
                break;
            }
            PrintOption(&longopts[option_index]);
            optset[option_index] = true;
        }
        if(stop)
            break;
    }
    return 0;
}