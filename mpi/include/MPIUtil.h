#ifndef __MPIUTIL_H__
#define __MPIUTIL_H__
#include <mpi.h>
#include <string>

namespace MPIUtil {
class Critical
{
    public:
        Critical(int owner = 0) {
            MPI_Win_create(&critical, sizeof(critical), sizeof(critical),
                    MPI_INFO_NULL, MPI_COMM_WORLD, &win);
            valid = true;
            this->owner = owner;
            Init();
        }
        ~Critical() {
            Destroy();
        }
        void Destroy() {
            if(valid)
            {
                MPI_Barrier(MPI_COMM_WORLD);
                MPI_Win_free(&win);
                valid = false;
            }
        }
        void Init() {
            int rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            if(rank == owner)
            {
                MPI_Win_lock_all(0, win);
                critical = 0;
                MPI_Win_sync(win);
                MPI_Win_unlock_all(win);
            }
        }
        void Enter() {
            int r = 1;
            int pos_one = 1;
            int zero = 0;
            MPI_Win_lock_all(0, win);
            while(r != 0)
            {
                MPI_Compare_and_swap(&pos_one, &zero, &r, MPI_INTEGER, owner, 0, win);
                MPI_Win_flush(0, win);
            }
            MPI_Win_unlock_all(win);
        }
        void Leave() {
            int r;
            int pos_one = 1;
            int zero = 0;
            MPI_Win_lock_all(0, win);
            MPI_Compare_and_swap(&zero, &pos_one, &r, MPI_INTEGER, owner, 0, win);
            MPI_Win_unlock_all(win);
        }
    private:
        MPI_Win win;
        int critical;
        int owner;
        bool valid;
};

class Environment
{
public:
    Environment() {
        MPI_Init(NULL, NULL);
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        char* buf = new char[MPI_MAX_PROCESSOR_NAME];
        int buf_len;
        MPI_Get_processor_name(buf, &buf_len);
        processor_name = buf;
        delete[] buf;
    }
    ~Environment() {
        MPI_Finalize();
    }
public:
    int world_rank;
    int world_size;
    std::string processor_name;
};

template <typename Type>
class SharedVariable
{
public:
    MPI_Datatype dtype;
    SharedVariable(int owner = 0){
        this->owner = owner;
        InitDatatype();
        MPI_Win_create(&value, sizeof(value), sizeof(value),
                       MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    }
    ~SharedVariable() {
        MPI_Win_free(&win);
    }
    void InitDatatype() {
        dtype = MPI_BYTE;
    }
    Type& Get() {
        MPI_Get(&value, sizeof(value), dtype, owner, 0, 1, dtype, win);
        return value;
    }
    Type& Set(const Type& rhs) {
        value = rhs;
        MPI_Put(&value, 1, dtype, owner, 1, dtype, win);
        return value;
    }
private:
    int owner;
    Type value;
    MPI_Win win;
};

extern Environment mpienv;
} // namespace MPIUtil

#endif

