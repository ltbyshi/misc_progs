# 集群使用指南

## Grid Engine队列管理系统简介
Sun Grid Engine是一个集群队列管理软件，具有计算节点状态监控，任务调度，任务监控等功能。
目前安装的队列管理系统是[Son of Grid Engine](https://arc.liv.ac.uk/trac/SGE) 8.1.9版本，安装路径为`/opt/sge`
关于SGE的详细使用方法，可以参考：<http://arc.liv.ac.uk/SGE/htmlman/manuals.html>或者通过`man`命令来查看帮助。

目前的集群包括一台管理节点master(10.0.0.54)和两台计算节点node01(10.0.0.112)、node02(10.0.0.117)。管理节点不参与计算。
在所有的节点均可以提交任务，而计算只能在node*上进行。

## 队列
集群上提交的任务首先会进入到队列中，然后SGE根据各台机器空闲的资源状态来分配节点。如果各台机器的资源均以用完，则无法再提交任务。
目前系统里有一个默认队列: all.q。

## SGE命令使用方法
SGE包括的主要命令都是以字母q开头的，常用的包括：`qsub`, `qstat`, `qdel`, `qhost`, `qconf`, `qacct`。

### 查看集群的所有节点的状态
运行
```bash
qhost
```
将会显示全部的计算节点，每台节点的CPU核数、内存、负载等信息。如果有一行全部显示为`-`，代表着改节点无法访问。
```
[root@master ~]#qhost
HOSTNAME                ARCH         NCPU NSOC NCOR NTHR  LOAD  MEMTOT  MEMUSE  SWAPTO  SWAPUS
----------------------------------------------------------------------------------------------
global                  -               -    -    -    -     -       -       -       -       -
node01                  lx-amd64       12    1    6   12  0.03   94.0G    6.5G    7.8G     0.0
node02                  lx-amd64       12    1    6   12  0.02   94.0G    3.0G    7.9G     0.0
```

### 登录到计算节点
运行
```bash
qlogin
```
将会登录到一台空余的节点，类似于SSH登录。如果所有计算节点都已经被占满，则无法使用`qlogin`命令。
```
[user1@master ~]$ qlogin
Your job 38 ("QLOGIN") has been submitted
waiting for interactive job to be scheduled ...
Your interactive job 38 has been successfully scheduled.
Establishing builtin session to host node02 ...
```

### 提交新任务(qsub)
首先新建一个文件（`test.sh`)：
```bash
#!/bin/bash
#$ -S /bin/bash
#$ -cwd
#$ -N test
#$ -e test.stderr.log
#$ -o test.stdout.log
pwd
hostname
date
sleep 20
date
echo “send message to stderr” 1>&2
```
保存之后，运行：
```
[user1@master ~]$ qsub test.sh
Your job 61 ("test") has been submitted
```
显示任务成功被提交，默认将会被提交到all.q队列中。

**各个选项的含义**

脚本的开头以#$开头的选项都是用于指定qsub命令行参数的，主要包含以下选项：

| 选项 | 含义 |
|------|------|
| -S   | 脚本解释器，一般是/bin/bash。如果不是bash脚本的话，需要指定其他解释器。
| -cwd | 任务提交后将在当前目录运行，所有相对路径均相对于当前目录。
| -N 　| 任务的名称，便于查找任务。
| -e  | stderr（标准错误输出）输出文件路径。
| -o  | stdout（标准输出）输出文件路径。
| -q  | 队列名称，默认为all.q

### 查看任务提交状态(qstat)
显示任务状态
```
[user1@master ~]$ qstat
job-ID  prior   name       user         state submit/start at     queue                          slots ja-task-ID
-----------------------------------------------------------------------------------------------------------------
     39 0.00000 test       user1        qw    03/29/2017 19:54:38                                    1
```
其中，第一列表示的任务的ID。state为任务状态：状态为`qw`代表任务在队列中等待；`r`代表任务正在运行；`Eqw`代表提交的脚本有问题。

如果任务已经完成，则qstat将不会显示该任务。

### 查看任务详细信息
如果一个任务已经在运行，则可通过以下命令查看详细信息
```
[user1@master ~]$ qstat -j 62
==============================================================
job_number:                 62
exec_file:                  job_scripts/62
submission_time:            Wed Mar 29 20:25:25 2017
owner:                      user1
uid:                        1001
group:                      user1
gid:                        1001
sge_o_home:                 /home/user1
sge_o_log_name:             user1
sge_o_path:                 /opt/sge/bin:/opt/sge/bin/lx-amd64:/usr/lib64/mpich/bin:/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/home/user1/.local/bin:/home/user1/bin
sge_o_shell:                /bin/bash
sge_o_workdir:              /home/user1
sge_o_host:                 master
account:                    sge
...
```

### 查看其他用户的任务
运行：
```
[user1@master ~]$ qstat -u '*'
job-ID  prior   name       user         state submit/start at     queue                          slots ja-task-ID
-----------------------------------------------------------------------------------------------------------------
     56 0.55500 test       user1        r     03/29/2017 20:21:43 all.q@master                       1
     57 0.55500 test       user1        r     03/29/2017 20:21:43 all.q@node02                       1
     58 0.55500 test       user1        r     03/29/2017 20:21:43 all.q@node01                       1
```
将显示所有用户的任务状态

### 查看队列资源使用状态
```
[user1@master ~]$ qstat -g c
CLUSTER QUEUE                   CQLOAD   USED    RES  AVAIL  TOTAL aoACDS  cdsuE
--------------------------------------------------------------------------------
all.q                             0.01      0      0      3      3      0      0
```

### 查看每台节点的任务状态
```
[user1@master ~]$ qstat -f
queuename                      qtype resv/used/tot. load_avg arch          states
---------------------------------------------------------------------------------
all.q@master                   BIP   0/0/1          0.03     lx-amd64
---------------------------------------------------------------------------------
all.q@node01                   BIP   0/0/1          0.01     lx-amd64
---------------------------------------------------------------------------------
all.q@node02                   BIP   0/0/1          0.01     lx-amd64

############################################################################
 - PENDING JOBS - PENDING JOBS - PENDING JOBS - PENDING JOBS - PENDING JOBS
############################################################################
     63 0.55500 test       user1        qw    03/29/2017 20:43:01     1
```
显示一个队列中全部的核数（TOTAL）、可用（AVAIL）和负载信息。

### 查看已经完成的任务 (qacct)
运行命令（需指定任务ID）：
```
[user1@master ~]$ qacct -j 56
==============================================================
qname        all.q
hostname     master
group        user1
owner        user1
project      NONE
department   defaultdepartment
jobname      test
jobnumber    56
taskid       undefined
account      sge
priority     0
qsub_time    Wed Mar 29 20:21:40 2017
start_time   Wed Mar 29 20:21:43 2017
end_time     Wed Mar 29 20:22:03 2017
...
```
将会显示类似以下的详细信息：
```
==============================================================
qname        all.q
hostname     node01
group        user1
owner        user1
project      NONE
department   defaultdepartment
jobname      test
jobnumber    39
taskid       undefined
account      sge
priority     0
qsub_time    Thu Jan  1 08:00:00 1970
```
该信息有助于了解任务完成的时间，资源利用情况等信息。

### 删除已经提交的任务 (qdel)

#### 根据ID删除任务
运行命令（需指定任务ID，如果有多个，可以用逗号分隔开）
```
qdel 39
```

#### 根据任务名称删除任务
如果在`qsub`提交时用`-N`选项指定了任务名称，则可以根据名称删除与该名称匹配的全部任务：
```
qdel test
```

在需要批量删除一组任务时，按照名称删除更加方便，而且不需要查找任务ID。

#### 删除一个用户提交的全部任务
运行：
```
qdel -u user1
```
可以删除用户user1提交的全部任务。

### 提交多线程任务
在脚本文件的开头加上一行：
```bash
#$ -pe def_slot 4
```
可以为该任务分配4个核，也可以把4改为其他数字，注意不能超出一个节点所能提供的最大核数。

## 存储使用
共享存储为Lustre系统，由一台主节点MDS（node01)和多台存储节点OSS（node01及node02）组成。
MDS存储的是文件目录树、存储位置等信息，而OSS是实际存储文件的服务器。客户端访问文件时，首先与MDS通信，
查找到文件所在的OSS的位置，然后客户端直接与OSS通信，访问所需文件。之后对该文件的访问则不需要再通过MDS，
因此在OSS较多时不会因为频繁与MDS通信而造成网络上的瓶颈。

所有节点均已经挂载Lustre, 路径为：`/lustre`。客户端能看到只有一个目录，但实际上文件分散存储在不同的位置。
通过`lfs`命令可以管理Lustre存储。

### 查看Lustre空间利用状态
```
[root@master ~]#lfs df -h
UUID                       bytes        Used   Available Use% Mounted on
lustre-MDT0000_UUID       377.0G      376.9M      351.0G   0% /lustre[MDT:0]
lustre-OST0000_UUID         7.6T      205.7G        7.0T   3% /lustre[OST:0]
lustre-OST0001_UUID         8.1T      144.2G        7.6T   2% /lustre[OST:1]

filesystem summary:        15.7T      349.9G       14.6T   2% /lustre
```
可以看到文件平均分散在两台机器上，最后一行显示了整个系统的空间利用情况。

**注意** Lustre利用率过高会降低效率并且可能导致无法写入，此时应该删除文件或者增加OSS。

### 查看文件或目录所在的OSS：
查看当前目录：
```
[root@master lustre]#lfs getstripe .
.
stripe_count:   1 stripe_size:    1048576 stripe_offset:  -1
./users
stripe_count:   1 stripe_size:    1048576 stripe_offset:  -1
./common
stripe_count:   1 stripe_size:    1048576 stripe_offset:  -1
```
其中stripe_offset为-1表示由系统自动分配存储节点的位置。
查看文件：
```
[root@master common]#lfs getstripe ToolsDir.py
ToolsDir.py
lmm_stripe_count:   1
lmm_stripe_size:    1048576
lmm_pattern:        1
lmm_layout_gen:     0
lmm_stripe_offset:  0
	obdidx		 objid		 objid		 group
	     0	        309996	      0x4baec	             0
```
obdix=0代表着这个文件存储在OSS0 (node01)这台节点上。

如果要求某个目录的文件都存储在一台OSS上，可以运行：
```
lfs setstripe --count 1 --index 0 directory
```
把文件都放在OSS0这台机器上。

### 查看OST节点状态
```
[root@master ~]#lfs osts
OBDS:
0: lustre-OST0000_UUID ACTIVE
1: lustre-OST0001_UUID ACTIVE
```

### 查看MDT节点状态
```
[root@master ~]#lfs mdts
MDTS:
0: lustre-MDT0000_UUID ACTIVE
```

### 查看用户的quota以及使用的空间
```
[root@master ~]#lfs quota -u root /lustre
Disk quotas for user root (uid 0):
     Filesystem  kbytes   quota   limit   grace   files   quota   limit   grace
        /lustre  476320       0       0       -     270       0       0       -
```
注意-u选项后面为用户名。其中kbytes表示是以KB为单位的空间使用，files是文件总数。

必要时可以用`lfs setquota`命令限制某个用户的存储使用。

## 注意事项
* 如果一个节点的负载（用`qhost`命令查看）超过该节点核数的1.75倍，则提交的任务必须等待，所以在有多线程任务时尽量用`-pe`选项指定核数，防止节点超载。
* 主节点（master）重启不影响计算节点上的任务，但是可能会影响在队列中等待的任务。
