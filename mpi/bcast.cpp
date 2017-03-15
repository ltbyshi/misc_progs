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
int root = 0;

size_t block_size = (1UL << 20);
size_t block_count = 1024;
size_t data_size;

int CheckSum(char* data, size_t size)
{
    int sum = 0;
    for(size_t i = 0; i < size; i ++)
    {
        sum += (data[i] & 1);
    }
    return sum;
}

void Broadcast(char* data)
{
    for(size_t i = 0; i < block_count; i ++)
        MPI_Bcast(data + i*block_size, block_size, MPI_BYTE, root, MPI_COMM_WORLD); 
    cout << "[" << processor_name << "] bcast(" << root << ", " << world_rank << "): " 
         << block_size*block_count << " bytes" << endl;
}

int main()
{
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Get_processor_name(processor_name, &name_len);
    data_size = block_size*block_count;

    if(world_size < 2)
    {
        cerr << "Need at least two processes (given " << world_size << ")" << endl;
        abort();
    }


    cout << "Data size: " << data_size << " bytes" << endl;
    char* data = new char[data_size];
    TIMEIT(Broadcast(data));
    MPI_Finalize();
    delete[] data;

    return 0;
}

