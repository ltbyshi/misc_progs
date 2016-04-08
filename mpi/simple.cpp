#include <mpi.h>
#include <iostream>
#include <stdexcept>
using namespace std;
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

void MPISimple()
{
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    char processor_name[1024];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    MPI_Barrier(MPI_COMM_WORLD);
    cout << "Processor name: " << processor_name
         << ", pid: " << getpid() 
         << ", rank: " << world_rank
         << ", size: " << world_size << endl;
#if 1
    if(world_rank == (rand() % world_size))
    {
        exit(0);
        //throw runtime_error("too simple");
        //*((int*)0) = 0;
        //raise(SIGSEGV);
    }
#endif
    sleep(1);

    MPI_Finalize();

}
int main()
{
    srand(time(0));
    MPISimple();
    MPISimple();
    return 0;
}

