#! /bin/bash
#$ -S /bin/bash
#$ -cwd
#$ -N {JobName}
#$ -o {LogDir}/{JobName}.stdout.log
#$ -e {LogDir}/{JobName}.stderr.log
#$ -t 1-{NumJobs}

. ~/.bashrc

printf '' > {LogDir}/{JobName}.stdout.log
printf '' > {LogDir}/{JobName}.stderr.log

verbose_cmd(){
    echo '-----------------------------------------------------------------'
    echo $@
    echo '-----------------------------------------------------------------'
    $@
}

verbose_cmd date
verbose_cmd hostname

