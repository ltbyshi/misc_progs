#include <iostream>
#include <fstream>
using namespace std;
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

size_t nbytes = 0;
size_t nlines = 0;
double prev_time = 0;
unsigned int interval = 1;
const size_t bufsize = 1024UL*1024UL;

double GetTime()
{
    timeval tv;
    gettimeofday(&tv,NULL);
    return double(tv.tv_sec) + double(tv.tv_usec)*1e-6;
}

void PrintSpeed(int signum)
{
    if(signum == SIGALRM)
    {
        double cur_time = GetTime();
        double line_sp = double(nlines)/(cur_time - prev_time);
        double byte_sp = double(nbytes)/(cur_time - prev_time);
        fprintf(stderr, "%.1f lines/s, ", line_sp);
        if(byte_sp < 1024L) {
            fprintf(stderr, "%.2f B/s", byte_sp);
        } else if(byte_sp < (1UL << 20)){
            fprintf(stderr, "%.2f K/s", byte_sp/(1UL << 10));
        } else if(byte_sp < (1UL << 30)){
            fprintf(stderr, "%.2f M/s", byte_sp/(1UL << 20));
        } else if(byte_sp < (1UL << 40)){
            fprintf(stderr, "%.2f G/s", byte_sp/(1UL << 30));
        }
        fprintf(stderr, "\n");
        fflush(stderr);
        nbytes = 0;
        nlines = 0;
        prev_time = cur_time;
        alarm(interval);
    }
}

void CountLines(istream& fin)
{
    while(!fin.eof())
    {
        string line;
        getline(fin, line);
        nbytes += line.size() + 1;
        nlines ++;
    }
}

void CountBytes(istream& fin)
{
    char* buf = new char[bufsize];
    while(!fin.eof())
    {
        fin.read(buf, bufsize);
        nbytes += fin.gcount();
    }
    delete[] buf;
}

int main(int argc, char** argv)
{
    istream* is = NULL;
    if(argc == 2)
    {
        is = new ifstream(argv[1], ios::binary|ios::in);
    }
    else
    {
        is = &cin;
        freopen(NULL, "rb", stdin);
    }
    signal(SIGALRM, PrintSpeed);
    prev_time = GetTime();
    alarm(interval);
    //CountLines(fin);
    CountBytes(*is);
    PrintSpeed(SIGALRM);
    return 0;
}
