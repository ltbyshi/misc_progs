#! /usr/bin/env python

def parallelmap(func, args, n_jobs=None, engine='mpi'):
    if engine == 'mpi':
        import math
        from mpi4py import MPI
        comm = MPI.COMM_WORLD
        rank = comm.Get_rank()
        world_size = comm.Get_size()
        unit_size = int(math.ceil(float(len(args))/world_size))
        jobindex_start = unit_size*rank
        jobindex_end = min(jobindex_start + unit_size, len(args))
        result_unit = map(func, args[jobindex_start:jobindex_end])
        #print 'Rank {0}: {1}-{2}'.format(rank, jobindex_start, jobindex_end)
        comm.Barrier()
        result = comm.gather(result_unit, root=0)
        #if rank != 0:
        #    result = [0]*len(args)
        #comm.Bcast(result, root=0)
        if rank == 0:
            #print '[Rank {0}]: {1}'.format(rank, repr(result)) 
            result = reduce(lambda x, y: x + y, result)
        else:
            return None
        return result
    elif engine == 'multiprocessing':
        import multiprocessing
        pool = multiprocessing.Pool(n_jobs)
        result = pool.map(func, args)
        pool.close()
        pool.join()
        return result
    elif engine == 'python':
        return map(func, args)
    else:
        raise ValueError('Invalid parallel engine: {0}'.format(engine))

def Squared(x):
    return x*x

def Identity(x):
    return x

if __name__ == '__main__':
    import argparse, os, sys
    parser = argparse.ArgumentParser()
    parser.add_argument('-j', '--jobs', type=int, default=4, required=False)
    parser.add_argument('--engine', type=str, choices=('mpi', 'multiprocessing', 'python'),
                        default='multiprocessing', required=False)
    parser.add_argument('-n', type=int, default=23, required=False, help='data count')
    args = parser.parse_args()

    data = range(1, args.n + 1)
    print 'Data:  ',data
    print 'Result:',parallelmap(Identity, data, args.jobs, args.engine)
    print 'Result:',parallelmap(Identity, data, args.jobs, args.engine)

