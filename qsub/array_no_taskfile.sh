#! /bin/bash
#$ -S /bin/bash
#$ -N {JobName}
#$ -o {LogDir}/{JobName}/stdout.$TASK_ID.log
#$ -e {LogDir}/{JobName}/stderr.$TASK_ID.log

. ~/.bashrc

printf '' > {LogDir}/{JobName}/stdout.${SGE_TASK_ID}.log
printf '' > {LogDir}/{JobName}/stderr.${SGE_TASK_ID}.log

verbose_cmd(){
    echo '-----------------------------------------------------------------'
    echo $@
    echo '-----------------------------------------------------------------'
    $@
}

verbose_cmd date
verbose_cmd hostname
# begin of commands
