#! /usr/bin/env python
import string, argparse, os, sys

template_qsub = '''#! ${shell}
#$$ -S ${shell}
#$$ -cwd
#$$ -N ${name}
#$$ -o ${logdir}/${name}/stdout.$$TASK_ID.log
#$$ -e ${logdir}/${name}/stderr.$$TASK_ID.log
#$$ -t ${array}
#$$ -q ${queue}
#$$ -pe ${pe} ${nslots}
#$$ -l ${resource}

print_prolog(){
    date_str=`date`
    echo "--------------------------------------------------------"
    echo "[$$date_str] SGE job started. Job ID: $$JOB_ID, task ID: $$SGE_TASK_ID"
    echo "--------------------------------------------------------"
}
print_epilog(){
    date_str=`date`
    echo "========================================================"
    echo "[$$date_str] SGE job finished. Job ID: $$JOB_ID, task ID: $$SGE_TASK_ID"
    echo "========================================================"
}
# print message about the start of the job
print_prolog > ${logdir}/${name}/stderr.$$SGE_TASK_ID.log
print_prolog > ${logdir}/${name}/stdout.$$SGE_TASK_ID.log

# get number of tasks and task rank
export SGE_TASK_LAST=$${SGE_TASK_LAST:=1}
export SGE_TASK_FIRST=$${SGE_TASK_FIRST:=1}
export SGE_TASK_ID=$${SGE_TASK_ID:=1}
NUM_TASKS=$$(expr $$SGE_TASK_LAST - $$SGE_TASK_FIRST + 1)
RANK=$$SGE_TASK_ID

# run commands from a command list file
# only commands that belongs to the task will be run
run_tasks(){
    sed '/^\s*$$/ d' $$1 | sed "$$RANK~$$NUM_TASKS !d" \\
        | xargs -d '\\n' -l -I '{}' ${shell} -c '{}'
}

# main script
run_tasks ${task_file}
'''

template_bsub ='''#!${shell}
#BSUB -J "${name}[${array}]"
#BSUB -o ${logdir}/${name}/stdout.%I.log
#BSUB -e ${logdir}/${name}/stderr.%I.log
#BSUB -R "span[hosts=1]"
#BSUB -q ${queue}

print_prolog(){
    date_str=`date`
    echo "--------------------------------------------------------"
    echo "[$$date_str] LSF job started. Job ID: $$LSB_JOBID, task ID: $$LSB_JOBINDEX"
    echo "--------------------------------------------------------"
}
print_epilog(){
    date_str=`date`
    echo "========================================================"
    echo "[$$date_str] LSF job finished. Job ID: $$LSB_JOBID, task ID: $$LSB_JOBINDEX"
    echo "========================================================"
}
# print message about the start of the job
print_prolog
print_prolog >&2

# get number of tasks and task rank
export LSB_JOBINDEX_END=$${LSB_JOBINDEX_END:=1}
export LSB_JOBINDEX=$${LSB_JOBINDEX:=1}
NUM_TASKS=$$LSB_JOBINDEX_END
RANK=$$LSB_JOBINDEX

# run commands from a command list file
# only commands that belongs to the task will be run
run_tasks(){
    sed '/^\s*$$/ d' $$1 | sed "$$RANK~$$NUM_TASKS !d" \\
        | xargs -d '\\n' -l -I '{}' ${shell} -c '{}'
}

# main script
run_tasks ${task_file}
'''

epilog_qsub = '''
# print message about the end of the job
print_epilog
print_epilog >&2
'''

epilog_bsub = '''
# print message about the end of the job
print_epilog
print_epilog >&2
'''

def substitute_multiline(template, vardict, fout):
    for line in template.split('\n'):
        t = string.Template(line)
        try:
            fout.write(t.substitute(vardict))
            fout.write('\n')
        except KeyError:
            pass

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Generate qsub job scripts')
    parser.add_argument('-n', '--name', type=str, required=True,
            help='job name')
    parser.add_argument('-j', '--nslots', type=str, required=False, default=1,
            help='number of tasks in one job')
    parser.add_argument('-l', '--resource', type=str, required=False,
            help='request resource list')
    parser.add_argument('-a', '--array', type=str, required=False,
            default='1-1', help='generate array job, format: [task_id_first]-task_id_end')
    parser.add_argument('--logdir', type=str, required=False,
            default='Logs', help='directory for storing logs')
    parser.add_argument('--jobdir', type=str, required=False,
            default='Jobs', help='directory for storing job scripts')
    parser.add_argument('--shell', type=str, required=False,
            default='/bin/bash', help='shell command')
    parser.add_argument('--pe', type=str, required=False,
            default='make', help='parallel environment')
    parser.add_argument('--task-file', type=str, required=False,
            help='a text file containing one shell command per line')
    parser.add_argument('--print-script', action='store_true', required=False,
            help='print the generated job script')
    parser.add_argument('script_file', type=str, nargs='?',
            help='append script file to the end of the job script')
    parser.add_argument('--bsub', action='store_true', required=False,
            help='generate bsub job script rather than qsub')
    parser.add_argument('-q', '--queue', type=str, required=False,
            help='name of the queue to submit the job')
    args = parser.parse_args()

    logdir = os.path.join(args.logdir, args.name)
    if not os.path.exists(logdir):
        print 'Create log dir: %s'%logdir
        os.makedirs(logdir)
    if not os.path.exists(args.jobdir):
        print 'Create job dir: %s'%args.jobdir
        os.makedirs(args.jobdir)
    if args.bsub:
        template = template_bsub
        epilog = epilog_bsub
    else:
        template = template_qsub
        epilog = epilog_qsub

    job_file = os.path.join(args.jobdir, '%s.sh'%args.name)
    template_lines = template.split('\n')

    vardict = {k:v for k, v in vars(args).iteritems() if v is not None}
    with open(job_file, 'w') as fout:
        substitute_multiline(template, vardict, fout)
        if args.script_file:
            if args.script_file == '-':
                fin = sys.stdin
            else:
                fin = open(args.script_file, 'r')
            for line in fin:
                fout.write(line)
            fin.close()
        substitute_multiline(epilog, vardict, fout)
    fout.close()

    if args.print_script:
        with open(job_file, 'r') as f:
            for line in f:
                sys.stdout.write(line)


