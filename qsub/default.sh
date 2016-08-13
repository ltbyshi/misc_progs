#! /bin/bash
#$ -S /bin/bash
#$ -cwd
#$ -N {JobName}
#$ -o {LogDir}/{JobName}.stdout.log
#$ -e {LogDir}/{JobName}.stderr.log

. ~/.bashrc

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
# commands are here

