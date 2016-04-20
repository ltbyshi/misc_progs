#! /bin/bash
#$ -S /bin/bash
#$ -cwd
#$ -N {JobName}
#$ -o {LogDir}/{JobName}.stdout.log
#$ -e {LogDir}/{JobName}.stderr.log
#$ -pe mpi {NumJobs}

verbose_cmd(){
    echo '-----------------------------------------------------------------'
    echo $@
    echo '-----------------------------------------------------------------'
    $@
}

printf '' > {LogDir}/{JobName}.stdout.log
printf '' > {LogDir}/{JobName}.stderr.log

verbose_cmd date
verbose_cmd hostname
#verbose_cmd mpirun -np $NSLOTS ~/Tests/mpi/bin/simple
