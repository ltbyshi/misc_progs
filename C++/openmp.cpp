#include <iostream>
using namespace std;
#include <unistd.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

int main()
{
    srand(time(0));
    #pragma omp parallel
    {
        //sleep(1);
        cout << "Thread " << omp_get_thread_num() << endl;
    }
    return 0;
}


