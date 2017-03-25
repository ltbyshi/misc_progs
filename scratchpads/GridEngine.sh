#! /bin/bash
# Install GridEngine on CentOS 7
# Install EPEL 7: https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
wget 'https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm'
sudo yum localinstall epel-release-latest-7.noarch.rpm
sudo yum makecache
# Install the latest version of JRE
# Download from https://www.java.com/en/download/manual.jsp
sudo yum localinstall jre-8u121-linux-x64.rpm
# Install Son of Grid Engine
# Link: http://biohpc.blogspot.com/2016/10/sge-installation-of-son-of-grid-engine.html
# Refernces: http://www.softpanorama.org/HPC/Grid_engine/Installation/installation_of_execution_host.shtml
# Download the latest version of Son of Grid Engine from https://arc.liv.ac.uk/trac/SGE
#wget 'http://arc.liv.ac.uk/downloads/SGE/releases/8.1.9/sge-8.1.9.tar.gz'
wget 'http://arc.liv.ac.uk/downloads/SGE/releases/8.1.9/gridengine-8.1.9-1.el5.src.rpm'
# Install dependencies for building the package
sudo yum install jemalloc-devel munge-devel csh openssl-devel ncurses-devel pam-devel \
    net-tools libXmu-devel hwloc-devel libdb-devel java-devel javacc ant-junit motif-devel
rpmbuild --without java --rebuild -v gridengine-8.1.9-1.el5.src.rpm
# Install the RPMs of SGE
yum localinstall ~/rpmbuild/RPMS/x86_64/*.rpm
yum localinstall ~/rpmbuild/RPMS/noarch/*.rpm
# A user named sgeadmin will be created automatically

# Edit /etc/profile.d/sge.sh
# export SGE_ROOT=/opt/sge
# export PATH=/opt/sge/bin/lx-amd64:$PATH

# Edit /etc/hosts (domain name is needed)
# 192.168.56.108  vm1 vm1.centos7-2.vbox
# 192.168.56.109  vm2 vm2.centos7-2.vbox

# Edit /etc/hostname to change the hostnames
hostname <hostname>

# Install qmaster
cd /opt/sge
./install_qmaster
# Answer the questions

* Do you want to install Grid Engine
under a user ID other than >root< (y/n) [y] >> y
* Please enter a valid user name >> sgeadmin
* If this directory is not correct (e.g. it may contain an automounter
prefix) enter the correct path to this directory or hit <RETURN>
to use default [/opt/sge] >>
* sge_qmaster service set to port 6444
How do you want to configure the Grid Engine communication ports?
Using the >shell environment<:                           [1]
Using a network service like >/etc/service<, >NIS/NIS+<: [2]
(default: 2) >>
* sge_execd service set to port 6445
How do you want to configure the Grid Engine communication ports?
Using the >shell environment<:                           [1]
Using a network service like >/etc/service<, >NIS/NIS+<: [2]
(default: 2) >>
* The environment variable
   $SGE_CELL=<your_cell_name>
will be set for all further Grid Engine commands.
Enter cell name [default] >>
* The cluster name must start with a letter ([A-Za-z]), followed by letters,
digits ([0-9]), dashes (-) or underscores (_).
Enter new cluster name or hit <RETURN>
to use default [p6444] >> centos7_vbox
* Enter a qmaster spool directory [/opt/sge/default/spool/qmaster] >>
* Are you going to install Windows Execution Hosts? (y/n) [n] >> n
* Did you either install this version with a package manager,
or otherwise verify and set the file permissions of your
distribution (enter: y) (y/n) [y] >>
* Are all hosts of your cluster in a single DNS domain (y/n) [y] >>
* Do you want to enable the JMX MBean server (y/n) [n] >>
* Please choose a spooling method (berkeleydb|classic) [classic] >>
* Please enter a range [20000-20100] >>
* The pathname of the spool directory of the execution hosts. User >sgeadmin<
must have the right to create this directory and to write into it.
Default: [/opt/sge/default/spool] >>
* Please enter an email address in the form >user@foo.com<.
Default: [none] >>
* The following parameters for the cluster configuration were configured:
   execd_spool_dir        /opt/sge/default/spool
   administrator_mail     none
Do you want to change the configuration parameters (y/n) [n] >>
* We can install the startup script that will
start qmaster at machine boot (y/n) [y] >>
* Do you want to use a file which contains the list of hosts (y/n) [n] >>
* Adding admin and submit hosts
Stop by entering <RETURN>. You may repeat this step until you are
entering an empty list. You will see messages from Grid Engine
when the hosts are added.
Host(s): master
* Do you want to add your shadow host(s) now? (y/n) [n] >> n
* Configurations
--------------
1) Normal
          Fixed interval scheduling, report limited scheduling information,
          actual + assumed load
2) High
          Fixed interval scheduling, report limited scheduling information,
          actual load
3) Max
          Immediate Scheduling, report no scheduling information,
          actual load
Enter the number of your preferred configuration and hit <RETURN>!
Default configuration is [1] >>
* Do you want to see previous screen about using Grid Engine again (y/n) [n] >>

cp /opt/sge/default/common/settings.sh /etc/profile.d/sge.sh

# Install execd
./install_execd
# Answer the following questions
* If this directory is not correct (e.g. it may contain an automounter
prefix) enter the correct path to this directory or hit <RETURN>
to use default [/opt/sge] >>
* Please enter the cell name which you used for the qmaster
installation or press <RETURN> to use [default] >>
* The port for sge_execd is currently set as service.
   sge_execd service set to port 6445
* The spool directory is currently set to:
<</opt/sge/default/spool/master>>
Do you want to configure a different spool directory
for this host (y/n) [n] >>
* We can install the startup script that will
start execd at machine boot (y/n) [y] >>
* Do you want to add a default queue instance for this host (y/n) [y] >>
* root@master modified "@allhosts" in host group list
root@master modified "all.q" in cluster queue list
* Grid Engine startup scripts can be found at:
   /opt/sge/default/common/sgemaster (qmaster)
   /opt/sge/default/common/sgeexecd (execd)
Do you want to see previous screen about using Grid Engine again (y/n) [n] >
# Install execd for every execution host
# Setup environment variables at login
cp /opt/sge/default/common/settings.sh /etc/profile.d/sge.sh
# Add the execution host to administrative host on qmaster
qconf -ah <execution host>
# Run install_execd on the execution host:
./install_execd
# Configuration
# You may verify your administrative hosts with the command
qconf -sh
# You may add new administrative hosts with the command
qconf -ah <hostname>
# Show host group list
qconf -shgrp @allhosts
# group_name @allhosts
# hostlist master node01

# Add execution host to hostlist (separated by spaces)
qconf -mhgrp @allhosts

# Turn on scheduling information, set schedd_job_info to true
qconf -msconf
