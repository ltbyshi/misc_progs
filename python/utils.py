import os

def GenSGEScript(Commands, JobName, Slots=1):
    template = open('Templates/qsub_template.sh', 'rt').read()
    logDir = 'Logs'
    outDir = 'Jobs'
    script = template.format(Commands=Commands, JobName=JobName,
                          Slots=str(Slots),
                          Cwd=os.getcwd(),
                          ErrLog=os.path.join(logDir, JobName + '.err.log'),
                          OutLog=os.path.join(logDir, JobName + '.out.log'),
                          StatusLog=os.path.join(logDir, JobName + '.status.log'))
    open(os.path.join(outDir, JobName + '.sh'), 'wt').write(script)
    
