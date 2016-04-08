#include <mpi.h>
#include <iostream>
#include <stdexcept>
using namespace std;
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

int main()
{
    srand(time(0));
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if(world_size != 2)
    {
        cerr << "Need two processes, given " << world_size << endl;
        abort();
    }

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    char processor_name[1024];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    cout << "Processor name: " << processor_name
         << ", pid: " << getpid() 
         << ", rank: " << world_rank
         << ", size: " << world_size << endl;
    const int cycles = 10000;
    const int data_size = 64;
    char* data_s = new char[data_size];
    char* data_r = new char[data_size];

    timeval tv;
    double start_usec = 0, end_usec = 0;
    if(world_rank == 0)
    {
        gettimeofday(&tv, NULL);
        start_usec = tv.tv_sec*1e6 + tv.tv_usec;
    }

    int lock = 0;
    MPI_Win win;
    MPI_Win_create(&lock, sizeof(lock), sizeof(lock), MPI_INFO_NULL,
            MPI_COMM_WORLD, &win);

    MPI_Win_lock_all(0, win); 
    cout << "Locked" << endl;
    sleep(1);
    cout << "Unlock" << endl;
    MPI_Win_unlock_all(win);    
    MPI_Win_free(&win);

    if(world_rank == 0)
    {
        gettimeofday(&tv, NULL);
        end_usec = tv.tv_sec*1e6 + tv.tv_usec;
        cout << "Total ping-pong cycles: " << cycles << ", " 
             << (end_usec - start_usec)/1e6/cycles << " s/cycle." << endl;
    }

    MPI_Finalize();
    delete[] data_s;
    delete[] data_r;

    return 0;
}

