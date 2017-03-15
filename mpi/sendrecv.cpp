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

int world_size;
int world_rank;
char processor_name[1024];
int name_len;

int CheckSum(char* data, size_t size)
{
    int sum = 0;
    for(size_t i = 0; i < size; i ++)
    {
        sum += (data[i] & 1);
    }
    return sum;
}

void SendRecv(char* data, size_t block_size, size_t block_count, int sender, int receiver, bool reverse = false)
{
    for(size_t i = 0; i < block_count; i ++)
    {
        if(reverse)
        {
            MPI_Recv(data + i*block_size, block_size, MPI_BYTE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(data + i*block_size, block_size, MPI_BYTE, receiver, 0, MPI_COMM_WORLD);
        }
        else
        {
            MPI_Send(data + i*block_size, block_size, MPI_BYTE, receiver, 0, MPI_COMM_WORLD);
            MPI_Recv(data + i*block_size, block_size, MPI_BYTE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    cout << "[" << processor_name << "] send(" << world_rank << " => " << receiver << "): " 
         << block_size*block_count << " bytes" << endl;
    cout << "[" << processor_name << "] recv(" << sender << " <= " << world_rank << "): "
         << block_size*block_count << " bytes" << endl;
}
int main()
{
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Get_processor_name(processor_name, &name_len);

    if(world_size < 2)
    {
        cerr << "Need at least two processes (given " << world_size << ")" << endl;
        abort();
    }

    bool need_check = false;
    size_t block_size = (1UL << 20);
    size_t block_count = 1024;
    size_t data_size = block_size*block_count;

    cout << "Data size: " << data_size << " bytes" << endl;
    char* data = new char[data_size];
    if(world_rank == 0)
    {
        if(need_check)
            cout << "Check sum of data to be sent: " << CheckSum(data, data_size) << endl;
        TIMEIT(SendRecv(data, block_size, block_count, world_size - 1, 1, false));
        if(need_check)
            cout << "Check sum of received data: " << CheckSum(data, data_size) << endl;
    }
    else
    {
        int sender = world_rank - 1;
        int receiver = (world_rank + 1)%world_size;

        TIMEIT(SendRecv(data, block_size, block_count, sender, receiver, true));
        if(need_check)
            cout << "Check sum of received data: " << CheckSum(data, data_size) << endl;
    }
    MPI_Finalize();
    delete[] data;

    return 0;
}

