#include <mpi.h>
#include <iostream>
using namespace std;
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define TIMEIT(expr) \
    { \
        timeval tv; gettimeofday(&tv, NULL); double start_usec = tv.tv_sec*1e6 + tv.tv_usec; \
        expr; \
        gettimeofday(&tv, NULL); double end_usec = tv.tv_sec*1e6 + tv.tv_usec; \
        cout << "TIMEIT "#expr": " << (end_usec - start_usec)/1e6 << " s." << endl; \
    }

int CheckSum(char* data, size_t size)
{
    int sum = 0;
    for(size_t i = 0; i < size; i ++)
    {
        sum += (data[i] & 1);
    }
    return sum;
}

int main()
{
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    char processor_name[1024];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    if(world_size < 2)
    {
        cerr << "Need at least two processes (given " << world_size << ")" << endl;
        abort();
    }

    bool need_check = false;
    size_t data_size = (1UL << 20);

    cout << "Data size: " << data_size << " bytes" << endl;
    char* data = new char[data_size];
    if(world_rank == 0)
    {
        if(need_check)
            cout << "Check sum of data to be sent: " << CheckSum(data, data_size) << endl;
        TIMEIT(MPI_Send(data, data_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD));
        cout << "[" << processor_name << "] 0 => 1: " 
             << data_size << " bytes" << endl;
    }
    else
    {
        int sender = world_rank - 1;
        TIMEIT(MPI_Recv(data, data_size, MPI_BYTE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE));
        cout << "[" << processor_name << "] " 
             << world_rank << " <= " << sender << ": " 
             << data_size << " bytes" << endl;
        if(need_check)
            cout << "Check sum of received data: " << CheckSum(data, data_size) << endl;
        
        int receiver = (world_rank + 1)%world_size;
        TIMEIT(MPI_Send(data, data_size, MPI_BYTE, receiver, 0, MPI_COMM_WORLD));
        cout << "[" << processor_name << "] "
             << world_rank << " => " << receiver << ": " 
             << data_size << " bytes" << endl;
    }
    if(world_rank == 0)
    {
        TIMEIT(MPI_Recv(data, data_size, MPI_BYTE, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE));
        cout << "[" << processor_name << "] 0 <= " << world_size - 1 << ": "
             << data_size << " bytes" << endl;
    }
    MPI_Finalize();
    delete[] data;

    return 0;
}

