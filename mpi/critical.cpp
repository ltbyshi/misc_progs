#include <mpi.h>
#include <iostream>
#include <stdexcept>
using namespace std;
#include <unistd.h>
#include <stdlib.h>

#include <MPIUtil.h>

#define PRINTEXPR_PROC(expr)  cout << "[proc" << world_rank << "] "#expr << endl; expr;
#define PRINTEXPR(expr) cout << #expr << endl; expr;

static int pos_one = 1;
static int zero = 0;

using MPIUtil::mpienv;

void CriticalSection(int world_rank)
{
    int critical;
    MPI_Win win;
    MPI_Win_create(&critical, sizeof(critical), sizeof(critical), MPI_INFO_NULL,
                 MPI_COMM_WORLD, &win);
    
    MPI_Win_lock_all(0, win);
    if(world_rank == 0)
    {
        critical = 0;
        MPI_Win_sync(win);
    }
    MPI_Win_unlock_all(win);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Win_lock_all(0, win);
    int r;
    while(r != 0)
    {
        MPI_Compare_and_swap(&pos_one, &zero, &r, MPI_INTEGER, 0, 0, win);
        MPI_Win_flush(0, win);
    }
    // begin critical
    PRINTEXPR(sleep(1));
    // end critial
    MPI_Compare_and_swap(&zero, &pos_one, &r, MPI_INTEGER, 0, 0, win);
    MPI_Win_unlock_all(win);

    PRINTEXPR(MPI_Barrier(MPI_COMM_WORLD));
    PRINTEXPR(MPI_Win_free(&win));
}

int main()
{
    int world_rank = mpienv.world_rank;

    MPI_Barrier(MPI_COMM_WORLD);
    cout << "Processor name: " << mpienv.processor_name
         << ", rank: " << mpienv.world_rank
         << ", size: " << mpienv.world_size << endl;

    MPIUtil::Critical critical;
    PRINTEXPR_PROC(critical.Enter());
    // begin critical
    PRINTEXPR_PROC(sleep(1));
    // end critical
    PRINTEXPR_PROC(critical.Leave());

    return 0;
}

