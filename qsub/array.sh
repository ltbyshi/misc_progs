#! /bin/bash
#$ -S /bin/bash
#$ -N {JobName}
#$ -o {LogDir}/{JobName}/stdout.$TASK_ID.log
#$ -e {LogDir}/{JobName}/stderr.$TASK_ID.log
#$ -t 1-{NumJobs}

. ~/.bashrc

printf '' > {LogDir}/{JobName}/stdout.${SGE_TASK_ID}.log
printf '' > {LogDir}/{JobName}/stderr.${SGE_TASK_ID}.log

TaskFile={TaskFile}

get_tasks() {
    export SGE_TASK_LAST=${SGE_TASK_LAST:=1}
    export SGE_TASK_FIRST=${SGE_TASK_FIRST:=1}
    export SGE_TASK_ID=${SGE_TASK_ID:=1}
    NUM_WORKERS=$(expr $SGE_TASK_LAST - $SGE_TASK_FIRST + 1)
    RANK=$SGE_TASK_ID
    echo "NUM_WORKERS=$NUM_WORKERS, RANK=$RANK"
}

run_tasks(){
    sed '/^\s*$/ d' $TaskFile | sed "$RANK~$NUM_WORKERS !d" \
        | xargs -d '\n' -l -I '{}' bash -c '{}'
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
