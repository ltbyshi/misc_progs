#include <stdio.h>
#include <string>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using namespace std;

// options
const char* prefix = "RN_";

struct Arguments
{
    int numSeq;
    int minLength;
    int maxLength;
    int fixedLength;
    int wrap;
    char* alphabet;
    int alphaSize;
    char* format;
};

// Create an Arguments object and set default values
Arguments* init_args()
{
    Arguments* args = new Arguments;
    args->numSeq = 5000;
    args->minLength = 100;
    args->maxLength = 0;
    args->wrap = 0;
    args->alphaSize = 4;
    args->alphabet = new char[args->alphaSize + 1];
    strcpy(args->alphabet, "ATCG");
    args->format = new char[100];
    strcpy(args->format, "RN_%010d");
    
    return args;
}

void destroy_args(Arguments* args)
{
    if(args->alphabet)
        delete[] args->alphabet;
    if(args->format)
        delete[] args->format;
    delete args;
}

int parse_int(const char* s)
{
    return atoi(s);
}

void usage()
{
    printf("Usage: seqgen [-n numSeq] [-l minLength] [-m maxLength] [-w wrap]\n"
           "    [-a alphabet] [-f format]\n");
    exit(1);
}

void parse_args(Arguments* args, int argc, char** argv)
{
    int c;
    int status;
    opterr = 0;
    status = 1;
    while((c = getopt(argc, argv, "hn:l:m:w:a:f:")) != -1)
    {
        switch(c)
        {
            case 'h':
                usage(); break;
            case 'n':
                args->numSeq = parse_int(optarg); break;
            case 'l':
                args->minLength = parse_int(optarg); break;
            case 'm':
                args->maxLength = parse_int(optarg); break;
            case 'w':
                args->wrap = parse_int(optarg); break;
            case 'a':
                args->alphaSize = strlen(optarg);
                if(args->alphabet)
                    delete[] args->alphabet;
                args->alphabet = new char[args->alphaSize + 1];
                strcpy(args->alphabet, optarg);
                break;
            case 'f':
                strcpy(args->format, optarg);
                break;
            case '?':
                if(optopt == 'c')
                    fprintf (stderr, "Error: option -%c requires an argument.\n", optopt);
                else if(isprint(optopt))
                    fprintf (stderr, "Error: unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Error: unknown option character `\\x%x'.\n", optopt);
                status = 0;
                break;
            default:
                abort();
        }
        if(!status)
            break;
    }
    if(!status)
        usage();

    if(args->maxLength == 0)
        args->maxLength = args->minLength;
    if(args->minLength > args->maxLength)
    {
        fprintf(stderr, "Error: minLength(%d) > maxLength(%d)\n", args->minLength, args->maxLength);
        exit(1);
    }
}

void GenerateSequence(Arguments* args)
{
    char* seq = NULL;
    char* format = NULL;

    seq = new char[args->maxLength + 1];
    format = new char[strlen(args->format) + 4];
    strcpy(format, ">");
    strcat(format, args->format);
    strcat(format, "\n");

    char* fragment = NULL;
    if(args->wrap > 0)
        fragment = new char[args->wrap + 1];
    for(int i = 0; i < args->numSeq; i ++)
    {
        int length = args->maxLength;
        if(args->minLength < args->maxLength)
            length = rand() % (args->maxLength - args->minLength) + args->minLength;
        for(int j = 0; j < length; j ++)
            seq[j] = args->alphabet[rand() % args->alphaSize];
        seq[length] = 0;
        printf(format, i + 1); fflush(stdout);
        if(args->wrap > 0)
        {
            int p = 0;
            while(length - p >= args->wrap)
            {
                strncpy(fragment, &(seq[p]), args->wrap);
                fragment[args->wrap] = 0;
                printf("%s\n", fragment);
                fflush(stdout);
                p += args->wrap;
            }
            if(length > p)
            {
                strncpy(fragment, &(seq[p]), length - p);
                fragment[length - p] = 0;
                printf("%s\n", fragment);
                fflush(stdout);
            }
        }
        else
        {
            printf("%s\n", seq);
            fflush(stdout);
        }
    }
    delete[] seq;
    delete[] format;
    if(args->wrap > 0)
        delete[] fragment;
}


int main(int argc, char** argv)
{
    Arguments* args;

    args = init_args();
    parse_args(args, argc, argv);
    srand((unsigned int)time(0));
    GenerateSequence(args);
    destroy_args(args);

    return 0;
}
    
