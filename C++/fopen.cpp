#include <stdio.h>
#include <sys/time.h>

#include <iostream>
using namespace std;


void TimeIt(const char* message, void (*func)())
{
    cout << "Time it: " << message << endl;
    struct timeval tvstart;
    gettimeofday(&tvstart, NULL);
    
    func();
    
    struct timeval tvend;
    gettimeofday(&tvend, NULL);
    double totalus = ((1e6*tvend.tv_sec) + tvend.tv_usec - (1e6*tvstart.tv_sec) - tvstart.tv_usec);
    cout << "Total time: " << totalus/1e3 << "ms" << endl;
}

void OpenFileSpeed()
{
    int n = 10000000;
    for(int i = 0; i < n; i ++)
        fclose(fopen("file", "r"));
}


int main()
{
    TimeIt("fopen speed", OpenFileSpeed);
    return 0;
}
