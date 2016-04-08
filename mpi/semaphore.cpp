#include <mpi.h>
#include <iostream>
#include <stdexcept>
using namespace std;
#include <unistd.h>
#include <stdlib.h>

#define PRINTEXPR(expr) cout << "[proc" << world_rank << "] "#expr << endl; expr;

static int pos_one = 1;
static int neg_one = -1;
static int zero = 0;

void EmptyLoop(size_t n)
{
    for(size_t i = 0; i < n; i ++)
        rand();
}

int main()
{
    srand(time(0));
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

    int sema;
    MPI_Win win;
    MPI_Win_create(&sema, sizeof(sema), sizeof(sema), MPI_INFO_NULL,
                 MPI_COMM_WORLD, &win);
    
    MPI_Win_lock_all(0, win);
    if(world_rank == 0)
    {
        sema = 3;
        MPI_Win_sync(win);
    }
    MPI_Win_unlock_all(win);
    MPI_Barrier(MPI_COMM_WORLD);

    if(world_rank != 0)
    {
    MPI_Win_lock_all(0, win);
    int sema_local = 0;
    do {
        
        PRINTEXPR(MPI_Get(&sema_local, 1, MPI_INTEGER,
            0, 0, 1, MPI_INTEGER, win));
        PRINTEXPR(MPI_Win_flush(0, win));
        cout << "Process " << world_rank << " get semaphore: " << sema_local << endl;
    }while(sema_local <= 0);
    MPI_Win_unlock_all(win);
    
    MPI_Win_lock_all(0, win);
    PRINTEXPR(MPI_Accumulate(&neg_one, 1, MPI_INTEGER, 0,
            0, 1, MPI_INTEGER, MPI_SUM, win));
    //PRINTEXPR(MPI_Win_flush(0, win));
    MPI_Win_unlock_all(win);

    //PRINTEXPR(EmptyLoop(100000000));
    PRINTEXPR(sleep(2));

    MPI_Win_lock_all(0, win);
    PRINTEXPR(MPI_Accumulate(&pos_one, 1, MPI_INTEGER, 0,
            0, 1, MPI_INTEGER, MPI_SUM, win));
    //PRINTEXPR(MPI_Win_flush(0, win));
    MPI_Win_unlock_all(win);
    }

    PRINTEXPR(MPI_Barrier(MPI_COMM_WORLD));
    PRINTEXPR(MPI_Win_free(&win));
    PRINTEXPR(MPI_Finalize());
    return 0;
}

