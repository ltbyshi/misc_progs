#!/bin/bash
# Download e2fsprogs rpms from https://downloads.hpdd.intel.com/public/e2fsprogs/latest/el6/RPMS/x86_64/
# Download lustre server rpms from https://downloads.hpdd.intel.com/public/lustre/lustre-2.5.1/el6/server/RPMS/x86_64/
# Download lustre client srpms from https://downloads.hpdd.intel.com/public/lustre/lustre-2.5.1/el6/client/SRPMS/
sudo yum install xmlto asciidoc elfutils-libelf-devel elfutils-devel zlib-devel binutils-devel newt-devel python-devel audit-libs-devel perl-ExtUtils-Embed hmaccalc python-docutils libselinux-devel
rpmbuild --rebuild kernel-2.6.32-573.12.1.el6_lustre.src.rpm
# Install e2fsprogs
cd e2fsprogs/
sudo yum localinstall *.rpm
# install lustre server rpms except lustre-dkms lustre-osd-zfs lustre-osd-zfs-mount
cd lustre-2.8.0/server
sudo yum install *.rpm
sudo reboot
# Lustre 2.x manual: https://build.hpdd.intel.com/job/lustre-manual/lastSuccessfulBuild/artifact/lustre_manual.xhtml
# build lustre client rpm packages
rpmbuild --rebuild lustre-client-*.src.srpm

# create a /etc/modprobe.d/lustre.conf and specify the network interface for communication
# options lnet networks=tcp0(eth1)

# load the lustre kernel modules
sudo modprobe lnet
sudo modprobe lustre
# list NIDs in the network
lctl list_nids
# Disable SELinux
# Change /etc/selinux/config to set SELINUX=permissive
setenforce Permissive
# add the hostnames to /etc/hosts:
# 192.168.56.106    mds1
# 192.168.56.106    oss1
# Estimate the MGT space: 2 KiB/inode x 100 million inodes x 2 = 400 GiB ldiskfs MDT
# make the mgs and mdt (may replace --fsname= with other names)
mkfs.lustre --fsname=lustre --mdt --mgs --index 0 /dev/<block device>
# optionally add other MDTs (<nid> can be identified by `lctl list_nids`)
mkfs.lustre --fsname=lustre --mdt --mgsnode=<nid> --index 1 /dev/<block device>
# mount MDT
mount -t lustre /dev/<block device> <mount point>
# create a OST
mkfs.lustre --fsname=lustre --ost --mgsnode=<nid> --index 0 /dev/<block device>
# mount the OST
mount -t lustre /dev/<block device> <mount point>
# mount on the clients (<mgs node> is the NID of the MGS)
mount -o localflock -t lustre <mgs node>:/<fsname> <mount point>
# Optional: store all files in a directory on a specific OST
lfs setstripe -i start_ost -c 1 <dirname/filename>
# Optinoal: get quota
lfs quota <mount point>
# Optional: set quota for a user
lfs setquota -u <user> <mount point>
