import os
import sys
import sqlite3
import subprocess

sge_template='''#! /bin/bash
#$ -S /bin/bash
#$ -N {JobName}
#$ -cwd
#$ -e {ErrLog}
#$ -o {OutLog}
#$ -pe mpi {Slots}

# .bashrc
source $HOME/.bashrc

cd {Cwd}

{Commands}

echo 0 > {StatusLog}
'''

log_dir = 'Logs'
job_dir = 'Jobs'

def GenSGEScript(Commands, JobName, Slots=1, template=None):
    if not template:
        template = sge_template
    script = template.format(Commands=Commands, JobName=JobName,
                          Slots=str(Slots),
                          Cwd=os.getcwd(),
                          ErrLog=os.path.join(log_dir, JobName + '.err.log'),
                          OutLog=os.path.join(log_dir, JobName + '.out.log'),
                          StatusLog=os.path.join(log_dir, JobName + '.status.log'))
    open(os.path.join(job_dir, JobName + '.sh'), 'wt').write(script)

class SGEJobManager:
    def __init__(self, dbFile):
        self.dbFile = dbFile
        if not os.path.exists(dbFile):
            self.CreateDB()
        self.conn = sqlite3.connect(self.dbFile)
        self.conn.row_factory = sqlite3.Row
        self.cwd = os.getcwd()
        self.logDir = 'Logs'
        self.scriptDir = 'Jobs'
        self.templFile = 'Templates/qsub_template.sh'
        
    def CreateDB(self):
        conn = sqlite3.connect(self.dbFile)
        cursor = conn.cursor()
        cursor.execute('''DROP TABLE IF EXISTS JobGroup''')
        cursor.execute('''CREATE TABLE JobGroup
            (JobGroupId INTEGER PRIMARY KEY AUTOINCREMENT,
            JobGroupName TEXT UNIQUE, Description TEXT)''')
        conn.commit()
        cursor.execute('''DROP TABLE IF EXISTS Job''')
        cursor.execute('''CREATE TABLE Job
            (JobId INTEGER PRIMARY KEY AUTOINCREMENT,
            JobGroupId INTEGER REFERENCES JobGroup(JobGroupId),
            JobName TEXT UNIQUE, SchedId INTEGER, Status TEXT,
            SchedJobName TEXT, Cwd TEXT, Slots INTEGER,
            Script TEXT, ErrLog TEXT, OutLog TEXT, StatusLog TEXT)''')
        conn.commit()
        conn.close()
    
    def GetGroupIdByName(self, JobGroupName):
        cursor = self.conn.cursor()
        cursor.execute('''SELECT JobGroupId FROM JobGroup
            WHERE JobGroupName == '%s' '''%JobGroupName)
        if cursor.rowcount < 1:
            sys.stderr.write('Job group does not exist: %s\n'%JobGroupName)
            return None
        return cursor.fetchone()['JobGroupId']
        
    
    def SubmitJob(self, JobGroupName, JobName, Commands, Slots=1):
        cursor = self.conn.cursor()

        job = {}
        job['JobGroupId'] = self.GetGroupIdByName(JobGroupName)
        job['JobName'] = JobName
        job['SchedId'] = -1
        job['Status'] = 'ready'
        job['SchedJobName'] = job['JobName'] + '.' + job['JobGroupName']
        job['Cwd'] = self.cwd
        job['Slots'] = Slots
        job['Script'] = os.path.join(self.scriptDir, JobGroupName, JobName + '.sh')
        job['ErrLog'] = os.path.join(self.logDir, JobGroupName, JobName + '.err.log')
        job['OutLog'] = os.path.join(self.logDir, JobGroupName, JobName + '.out.log')
        job['StatusLog'] = os.path.join(self.logDir, JobGroupName, JobName + '.status.log')
        job['Commands'] = Commands
        job['JobGroupName'] = JobGroupName
        
        # generate script
        template = open(self.templFile, 'rt').read()
        script = template.format(**job)
        #open(job.Script, 'wt').write(script)
        
        cursor.execute('''INSERT INTO Job 
            (JobGroupId, JobName, SchedId, Status, SchedJobName Cwd, Slots, Script, ErrLog, OutLog, StatusLog)
            VALUES ({JobGroupId}, {JobName}, {SchedId}, {Status}, {SchedJobName},
            {Cwd}, {Slots}, {Script}, {ErrLog}, {OutLog}, {StatusLog})'''.format(job))
        self.conn.commit()
        cursor.close()
    
    def CreateJobGroup(self, JobGroupName, description=''):
        cursor = self.conn.cursor()
        cursor.execute('''SELECT * FROM JobGroup
            WHERE JobGroupName == '%s' '''%JobGroupName)
        rows = cursor.fetchall()
        if rows:
            sys.stderr.write('Job group %s already exists\n'%JobGroupName)
            sys.exit(-1)
        cursor.execute('''INSERT INTO JobGroup (JobGroupName, Description)
            VALUES ('%s', '%s')'''%(JobGroupName, description))
        self.conn.commit()
        cursor.close()
        
    def ShowJobs(self, columns):
        cursor = self.conn.cursor()
        cursor.execute('''SELECT %s FROM Job, JobGroup
            WHERE Job.JobGroupId == JobGroup.JobGroupId'''%', '.join(columns))
        sys.stdout.write('\t'.join(columns) + '\n')
        for row in cursor.fetchall():
            sys.stdout.write('\t'.join([str(field) for field in row]) + '\n')
    
    def ShowJobGroups(self):
        cursor = self.conn.cursor()
        cursor.execute('''SELECT JobGroupName, Description FROM JobGroup''')
        sys.stdout.write('\t'.join(['JobGroupName', 'Description']) + '\n')
        for row in cursor.fetchall():
            sys.stdout.write('\t'.join([str(field) for field in row]) + '\n')
            
    def Clear(self):
        cursor = self.conn.cursor()
        cursor.execute('''DELETE FROM JobGroup''')
        cursor.execute('''DELETE FROM Job''')
        self.conn.commit()
        
    def ClearGroup(self, JobGroupName):
        JobGroupId = self.GetGroupIdByName(JobGroupName)
        cursor = self.conn.cursor()
        cursor.execute('''DELETE FROM Job
            WHERE JobGroupId == %d '''%JobGroupId)
        self.conn.commit()
        cursor.close()
        
    def TrackStatus(self):
        cursor = self.conn.cursor()
        
        # check qstat
        p = subprocess.Popen('qstat -xml', stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate()
        if out:
            import xml.etree.ElementTree as ET
            tree = ET.parse(out)
            root = tree.getroot()
            for job_list in root.iter('job_list'):
                SchedId = int(job_list.find('JB_job_number').text)
                jobName = job_list.find('JB_name').text
                state = job_list.find('state').text
                queueName = job_list.find('queue_name').text
                cursor.execute('''SELECT JobId FROM Job WHERE SchedJobName == '%s' '''%jobName)
                if cursor.rowcount > 0:
                    row = cursor.fetchone()
                    JobId = row[0]
                    if state == 'r':
                        Status = 'running'
                    elif state.find('q') >= 0:
                        Status = 'queued'
                    elif state.find('E') >= 0:
                        Status = 'error'
                    else:
                        Status = 'unknown'
                    cursor.execute('''UPDATE Job SET Status='%s', SchedId='%d'
                        WHERE JobId == '%d' '''%(Status, SchedId, JobId))
        # check status statuslog
        cursor.execute('''SELECT JobId, Status, StatusLog FROM Job''')
        for row in cursor.fetchall():
            if os.path.exists(row['StatusLog']):
                content = open(row['StatusLog'], 'rt').read()
                if content.strip() == '0':
                    cursor.execute('''UPDATE Job SET Status='success' WHERE JobId == %d'''%row['JobId'])
                else:
                    # failure if the job cannot be tracked
                    if row['Status'] == 'submitted':
                        cursor.execute('''UPDATE Job SET Status='failure' WHERE JobId == %d'''%row['JobId'])
        
                
        