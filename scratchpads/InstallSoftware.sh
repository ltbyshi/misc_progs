# OpenSSL: https://www.openssl.org/source/openssl-1.0.2k.tar.gz
set -e
# Installation root directory
prefix=$HOME/pkgs/openssh/7.5p1
wget -O openssl-1.0.2k.tar.gz https://www.openssl.org/source/openssl-1.0.2k.tar.gz
tar zxf openssl-1.0.2k.tar.gz
cd openssl-1.0.2k/
./Configure --prefix=$prefix
./config --prefix=$prefix shared
make
make install
# OpenSSH: http://mirrors.mit.edu/pub/OpenBSD/OpenSSH/portable/openssh-7.5p1.tar.gz
wget -O openssh-7.5p1.tar.gz http://mirrors.mit.edu/pub/OpenBSD/OpenSSH/portable/openssh-7.5p1.tar.gz
tar zxf openssh-7.5p1.tar.gz
cd openssh-7.5p1/
export LD_LIBRARY_PATH=$prefix/lib
[[ -d $prefix/var/run ]] || mkdir -p $prefix/var/run
[[ -d $prefix/var/log ]] || mkdir -p $prefix/var/log
./configure --prefix=$prefix \
    --with-ssl-dir=$prefix \
    --with-pid-dir=$prefix/var/run
make
make install
# Run OpenSSH, replace $port with an unassigned port number
$prefix/sbin/sshd -d -p $port -D -E $prefix/var/log/openssh.log

# Then create a bsub script (bsub.sh)
#---------------- start of script ----------------
#! /bin/bash
#BSUB -J "sshd"
#BSUB -R "span[hosts=1]"
#BSUB -q Z-LU

$HOME/pkgs/openssh/7.5p1/sbin/sshd -d -p $port -D -E $HOME/pkgs/openssh/7.5p1/var/log/openssh.log
#---------------- end of script ----------------
# And submit the job
bsub < bsub.sh
# After the job is running, identify the node (e.g. node504) and then login
ssh -p $port node504

# Use system sshd (set $prefix to the configuration directory of sshd)
prefix=$HOME/apps/sshd
mkdir -p $prefix
cd $prefix
mkdir -p etc var/run var/log
# Generate sshd host keys
ssh-keygen -t ed25519 -N '' -f etc/ssh_host_ed25519_key
ssh-keygen -t dsa -N '' -f etc/ssh_host_dsa_key
ssh-keygen -t ecdsa -N '' -f etc/ssh_host_ecdsa_key
ssh-keygen -t rsa -N '' -f etc/ssh_host_rsa_key
# Generate sshd_config
{
echo HostKey $prefix/etc/ssh_host_rsa_key
echo HostKey $prefix/etc/ssh_host_dsa_key
echo HostKey $prefix/etc/ssh_host_ecdsa_key
echo HostKey $prefix/etc/ssh_host_ed25519_key
echo AuthorizedKeysFile  .ssh/authorized_keys
PidFile $prefix/var/run/sshd.pid
} > etc/sshd_config
chmod 600 etc/sshd_config

# Run sshd
/usr/sbin/sshd -f $prefix/sshd_config -d -p $port -D -E $prefix/var/log/openssh.log
# Then create a bsub script (bsub.sh)
#---------------- start of script ----------------
#! /bin/bash
#BSUB -J "sshd"
#BSUB -R "span[hosts=1]"
#BSUB -q Z-LU

/usr/sbin/sshd -d -p $port -D -E $prefix/var/log/openssh.log
#---------------- end of script ----------------
