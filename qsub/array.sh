#! /bin/bash
#$ -S /bin/bash
#$ -cwd
#$ -N {JobName}
#$ -o {LogDir}/{JobName}.stdout.log
#$ -e {LogDir}/{JobName}.stderr.log
#$ -t 1-{NumJobs}

printf '' > {LogDir}/{JobName}.stdout.log
printf '' > {LogDir}/{JobName}.stderr.log

TaskFile={TaskFile}

get_tasks() {
    #NUM_TASKS=$(sed '/^\s*$/ d' $TaskFile | wc -l)
    NUM_WORKERS=$(expr $SGE_TASK_LAST - $SGE_TASK_FIRST + 1)
    RANK=$SGE_TASK_ID
    #NTASKS_JOB=`python -c "import math; print int(math.ceil(float($NTASKS) / $NWORKERS ))"`
    #TASK_MIN=`python -c "print ($SGE_TASK_ID - $SGE_TASK_FIRST) * $NTASKS_JOB + 1"`
    #TASK_MAX=`python -c "print min($TASK_MIN + $NTASKS_JOB - 1, $NTASKS)"`
    #echo "SGE_TASK_FIRST=$SGE_TASK_FIRST, SGE_TASK_LAST=$SGE_TASK_LAST, SGE_TASK_ID=$SGE_TASK_ID"
    echo "NUM_WORKERS=$NUM_WORKERS, RANK=$RANK"
}

run_tasks(){
    #for i in `seq $TASK_MIN $TASK_MAX`;do
    #    sed '/^\s*$/ d' $TaskFile | sed "$i !d" | bash
    #done
    sed '/^\s*$/ d' $TaskFile | sed "$RANK~$NUM_WORKERS !d" | bash
}

verbose_cmd(){
    echo '-----------------------------------------------------------------'
    echo $@
    echo '-----------------------------------------------------------------'
    $@
}

#verbose_cmd lscpu
verbose_cmd date
verbose_cmd hostname
verbose_cmd get_tasks
verbose_cmd run_tasks
