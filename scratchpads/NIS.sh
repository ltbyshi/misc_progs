#! /bin/bash
# NIS Configuration
# https://www.server-world.info/en/note?os=CentOS_6&p=nis&f=2
# Install on NIS client
sudo yum install ypbind rpcbind
# Install on the NIS server
sudo yum install ypserv
systemctl start ypserv.service
# set NIS domain name
ypdomainname centos7-2.vbox
echo "NISDOMAIN=centos7-2.vbox" >> /etc/sysconfig/network
# Enable NIS and update NIS server
authconfig --enablenis --nisdomain=centos7-2.vbox --nisserver=master.centos7-2.vbox --update
#
# add optionally if you need ( create home directory automatically if it's none )
