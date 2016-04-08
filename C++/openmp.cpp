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
    #pragma omp parallel num_threads(10)
    {
        //sleep(1);
        cout << "Thread " << omp_get_thread_num() << endl;
        if((rand() % 3) == 0)
            *((int*)0) = 0;
    }
    return 0;
}


