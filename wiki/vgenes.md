# 中科晶云集群配置日志
**Enable passwordless SSH access between servers**
Use `ssh-keygen` to generate RSA key pairs.

**/root/.ssh/authorized_keys**
```
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCvkrfoGV2Chjhn0HIE5ImN1jEm96A5nQOapUIugNCez7FOh90eMBnTilcX0nGXO1rcaHWKzHViqkyL6rgfommcsIDQ1UrJBMJNfF+h9CdKWjg0Bs7I6TrQJm9cbI1b7ymvYeIusZOd1nSRNBKm2H32G4aKHGC4FeCjN+IqVSRSKSrm/NEw+nr8oJlK+YjnYA/XOLb5HwwZiEaMbODyS0xT33QJeKQxLukB7/tDQ0YvVMKcEXjcNpx9LZOGHIm53OYSufxA+lC7/wDYsbk2hontvxqfXxV4TEAEoAO8Huy2EQmAis1C0z2dvI0sZF+F5UqWr5JnuuHMmHlVe3M8MHQv root@gene.cluster.com
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCkxu42Zqys2tTcq72JKlxgpmOXV+D1XWXuAY231VhJqJqd6CPCziQsrJAw3n9PbtP7iJLNjq183ATZ9DD9ESnhwJYS5TwEkjSoWzd6aOuy2ltSOJpaLj+ITSp8pDSnaRsTzaDQl0155PXqFW3Z27nx6dGsLwHzxeYQdFYPGJ01rPlLTAbkXomYVitvjkdgwXvrwLjrayQ1PxG9yTiz8KPmimL5873y1SXQWrtXTNDXTj1CD6S2up5wNWW6Jk4CK9pmkbmJs/d0vM4cxGQDcWP+60AEgmLeYsW7gTUh9yr5mP5QtI9t3TT3EuP1xVffVEh8fKPSShvwtIHgs55jr9wf root@node01.cluster.com
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC7wlqcqrdnNxMYFBzlqBC1cW4qJJKYJTEkyKczGHENqr8Xp2nmuU1GgD8w+pB/6H9/EqpUp+8JGyHYoz+tX60zO835CIr5d0gCFC4Z0uvGYS679VQ63aNT+bp1Zo32n03xJMcFZhnLDIhrNAs0YDCUnC4dicpXEeNUlaundMjoW44TexpiAaMbN3/pfZbJkkY35NifwZ5IOu74N4K3GF13Bo/KPkO8NQR0wC0unErQF6HQ0xFZCW2Orfh7T2RHLwYu+ORb7i6GRBBzCH+4d/1lfHoD/Nxs0rgiokc8baF/N/nGFl0To9ELAr5v4GHy1mstkSL9W+lbsHppPvtM5W+l root@node02.cluster.com
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCxZ0PtQWyVK2kLxzSaJ58Y6n/lE4gGZ3PqQxgs6W1k4F5av1UIUNKVqxk7UPqJ6eIBnvPuFCpcETCREw6KsAeyqAu0AeCnNk9KiG0FqjKSmGdeUTvyNj8mLMRDEKDiRcj4CI6UIPodlKNocUF+WXhUFgtRX/rnq40iMf85uB22A13QzBkb9J2y3i+5vTFhqjppJrOBu9X3eYPICSQHYBFDjmPurrZlGEo5z5rggMwA/VjyxmvuDNuQTTsOUfWFbqE5uW7rDR8azycabrwHh4Ll+kqJB2uyqyEsBgT4FJNSaxvc4BIfW8ZUamLrbE36tOKkzwsfGFn0k9wDra8Z+BF7 root@node03.cluster.com
```
**/etc/hosts**
```
10.0.0.107  gene gene.cluster.com
10.0.0.54   master  master.cluster.com
10.0.0.112  node01  node01.cluster.com  mds0 oss0
10.0.0.117  node02  node02.cluster.com  oss1
```

# Install Lustre software

## Disable SELinux on all machines
Edit /etc/sysconfig/selinux, change:
```
SELINUX=permissive
```

Install e2fsprogs 1.42.13.wc5-7 and lustre 2.9.0 server on mds0 and oss2

Partition /dev/sdb on mds0
```
parted /dev/sdb
parted) mklabel gpt primary ext4 1MiB 512GiB
parted) mklabel gpt primary ext4 512GiB 100%
parted) name 1 odt0
parted) name 2 ost0
parted) quit
```
Get NIDs:
```
mds0: 10.0.0.112@tcp
oss1: 10.0.0.117@tcp
```

Edit /etc/modprobe.d/lustre.conf on mds0 and add
```
options lnet networks=tcp0(em1)
```

Format Lustre MDT and OST on mds0
```
mkfs.lustre --reformat --fsname=lustre --mdt --mgs --index 0 /dev/sdb1
mkfs.lustre --reformat --fsname=lustre --ost --mgsnode=mds0@tcp --index 0 /dev/sdb2
```
Mount Lustre server on mds0
```
mkdir /mnt/mdt
mount.lustre /dev/sdb1 /mnt/mdt
mkdir /mnt/ost
mount.lustre /dev/sdb2 /mnt/ost
```
Mount Lustre client on node02
```
mkdir /lustre
mount.lustre -o localflock mds0@tcp:/lustre /lustre
```
script file for mounting lustre
```
/root/admin/mount-lustre
```

Setup firewall for Lustre
```
systemctl disable firewalld.service
systemctl stop firewalld.service
```

Edit /etc/modprobe.d/lustre.conf on oss1 and add
```
options lnet networks=tcp0(em1)
```

Format lustre OST on oss1
```
mkfs.lustre --reformat --fsname=lustre --ost --mgsnode=mds0@tcp --index 1 /dev/sdb
```
Mount Lustre OST on oss1
```
mkdir /mnt/ost
mount.lustre /dev/sdb /mnt/ost
```
Compile Lustre client pakcages on gene
```
rpmbuild --rebuild -v lustre-2.9.0-1.src.rpm
```
Enable Lustre at startup
```
chkconfig --add lustre
chkconfig lustre on
chkconfig --add lnet
chkconfig lnet on
```
Start Lustre kernel modules
```
systemctl start lnet
systemctl start lustre
```
Lustre startup script for MGS server (mds0): **/etc/init.d/lustre-server***
```
#!/bin/bash
# chkconfig: 2345 20 80
# Required-Start: lnet lustre
# Mount the Lustre server

# Source function library.
. /etc/init.d/functions

start() {
    [ -d /mnt/lustre/mdt ] || mkdir -p /mnt/lustre/mdt
    mountpoint -q /mnt/lustre/mdt || mount.lustre /dev/sdb1 /mnt/lustre/mdt
    [ -d /mnt/lustre/ost ] || mkdir -p /mnt/lustre/ost
    mountpoint -q /mnt/lustre/ost || mount.lustre /dev/sdb2 /mnt/lustre/ost
    [ -d /lustre ] || mkdir /lustre
    mountpoint -q /lustre || mount.lustre -o localflock mds0@tcp:/lustre /lustre
}

stop() {
    umount /lustre
    umount /mnt/lustre/ost
}

case "$1" in
    start)
       start
       ;;
    stop)
       stop
       ;;
    restart)
       stop
       start
       ;;
    status)
       # code to check status of app comes here
       # example: status program_name
       ;;
    *)
       echo "Usage: $0 {start|stop|status|restart}"
esac

exit 0
```
Lustre startup script for OSS server (oss1): **/etc/init.d/lustre-server**
```
#!/bin/bash
# chkconfig: 2345 20 80
# Required-Start: lnet lustre
# Mount the Lustre server

# Source function library.
. /etc/init.d/functions

start() {
    [ -d /mnt/lustre/ost ] || mkdir -p /mnt/lustre/ost
    mountpoint -q /mnt/lustre/ost || mount.lustre /dev/sdb /mnt/lustre/ost
    [ -d /lustre ] || mkdir /lustre
    mountpoint -q /lustre || mount.lustre -o localflock mds0@tcp:/lustre /lustre
}

stop() {
    umount /lustre
    umount /mnt/lustre/ost
}

case "$1" in
    start)
       start
       ;;
    stop)
       stop
       ;;
    restart)
       stop
       start
       ;;
    status)
       # code to check status of app comes here
       # example: status program_name
       ;;
    *)
       echo "Usage: $0 {start|stop|status|restart}"
esac

exit 0
```
Lustre startup script for clients: **/etc/init.d/lustre-client**
```
#!/bin/bash
# chkconfig: 2345 20 80
# Required-Start: lnet lustre
# Mount the Lustre server

# Source function library.
. /etc/init.d/functions

start() {
    [ -d /lustre ] || mkdir /lustre
    mountpoint -q /lustre || mount.lustre -o localflock mds0@tcp:/lustre /lustre
}

stop() {
    umount /lustre
}

case "$1" in
    start)
       start
       ;;
    stop)
       stop
       ;;
    restart)
       stop
       start
       ;;
    status)
       # code to check status of app comes here
       # example: status program_name
       ;;
    *)
       echo "Usage: $0 {start|stop|status|restart}"
esac

exit 0
```

# Install Son of Grid Engine 8.1.9 on all nodes
## Install qmaster on master
```
cd /opt/sge
./install_qmaster
cp /opt/sge/default/common/settings.sh /etc/profile.d/sge.sh
```
Setup firewall to allow internal network access on gene
```
firewall-cmd --permanent --add-source 10.0.0.0/24
```
Cluster name: cluster, cell name: default

First Install EPEL 7: https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm

Then install SGE RPMs

Copy the common configuration directory to each execution host:
```
scp -r /opt/sge/default <exec node>:/opt/sge
ssh <exec node> rm -rf /opt/sge/default/spool
```
Also set permissions
```
chown -R sgeadmin:sgeadmin /opt/sge/default
```
# Install LDAP
Edit /etc/openldap/ldap.conf
```
BASE    dc=cluster,dc=com
URI     ldap://master.cluster.com
```

Manager DN: **cn=root,dc=cluster,dc=com**

Generate root password using slappasswd (noncode123)

**chpasswd.ldif**
```
dn: olcDatabase={0}config,cn=config
changetype: modify
add: olcRootPW
olcRootPW: {SSHA}qSlsx4gLpDxh29z7uv0qA9HSDkDZd2bC
```
Add chrootpw.ldif
```
ldapadd -Y EXTERNAL -H ldapi:/// -f chrootpw.ldif
```
Add schemas
```
ldapadd -Y EXTERNAL -H ldapi:/// -f /etc/openldap/schema/cosine.ldif
ldapadd -Y EXTERNAL -H ldapi:/// -f /etc/openldap/schema/nis.ldif
ldapadd -Y EXTERNAL -H ldapi:/// -f /etc/openldap/schema/inetorgperson.ldif
```
**chdomain.ldif** (password for manager: noncode123)
```
dn: olcDatabase={1}monitor,cn=config
changetype: modify
replace: olcAccess
olcAccess: {0}to * by dn.base="gidNumber=0+uidNumber=0,cn=peercred,cn=external,cn=auth"
  read by dn.base="cn=root,dc=cluster,dc=com" read by * none

dn: olcDatabase={2}hdb,cn=config
changetype: modify
replace: olcSuffix
olcSuffix: dc=cluster,dc=com

dn: olcDatabase={2}hdb,cn=config
changetype: modify
replace: olcRootDN
olcRootDN: cn=root,dc=cluster,dc=com

dn: olcDatabase={2}hdb,cn=config
changetype: modify
add: olcRootPW
olcRootPW: {SSHA}oGgxRWRrUFZ4WxlT7LUwPJmqIlpD88/x

dn: olcDatabase={2}hdb,cn=config
changetype: modify
add: olcAccess
olcAccess: {0}to attrs=userPassword,shadowLastChange by
  dn="cn=root,dc=cluster,dc=com" write by anonymous auth by self write by * none
olcAccess: {1}to dn.base="" by * read
olcAccess: {2}to * by dn="cn=root,dc=cluster,dc=com" write by * read
```
Add chdomain.ldif
```
ldapadd -Y EXTERNAL -H ldapi:/// -f chdomain.ldif
```

**basedomain.ldif**
```
dn: dc=cluster,dc=com
objectClass: top
objectClass: dcObject
objectclass: organization
o: Cluster

dn: cn=root,dc=cluster,dc=com
objectClass: organizationalRole
cn: root
description: Directory Manager

dn: ou=People,dc=cluster,dc=com
objectClass: organizationalUnit
ou: People

dn: ou=Group,dc=cluster,dc=com
objectClass: organizationalUnit
ou: Group
```
Add basedomain.ldif (requires manager password)
```
ldapadd -x -D cn=root,dc=cluster,dc=com -W -f basedomain.ldif
```
**passwd.ldif**

Add (requires manager password)
```
ldapadd -x -D cn=root,dc=cluster,dc=com -W -f passwd.user
```
**group.ldif**

Migration
```
export LDAP_BASEDN='dc=cluster,dc=com'
export LDAPHOST='gene.cluster.com'
cn=root,dc=cluster,dc=com
./migrate_passwd.pl passwd.user passwd.ldif
./migrate_group.pl group.user group.ldif
```
On clients, enable LDAP authentification
(option --enableforcelegacy is very import).
```
authconfig --enableldapauth --enablesysnetauth --enablemkhomedir --enableldap \
    --enableforcelegacy \
    --disablenis --disablesssd --ldapserver=ldap://master.cluster.com \
    --ldapbasedn='dc=cluster,dc=com' --update
```
Modify shell to /bin/bash
```
qconf -mq all.q
```
**Kernel versions**
```
kernel.x86_64                                       3.10.0-327.el7                                             @anaconda
Available Packages
kernel.x86_64                                       3.10.0-514.el7                                             base
kernel.x86_64                                       3.10.0-514.2.2.el7                                         updates
kernel.x86_64                                       3.10.0-514.6.1.el7                                         updates
kernel.x86_64                                       3.10.0-514.6.2.el7                                         updates
kernel.x86_64                                       3.10.0-514.10.2.el7                                        updates
```
phpldapadmin URL: http://10.0.0.54:80/ldapadmin

SSH forward: http://127.0.0.1:23711/ldapadmin/

Create a file containing a list of users (users.txt)
```
guojincheng
fangshuangsang
wangxinmei
sunliang
liaoshengyou
weibin
luochunlong
wuyang
zhengjiawen
fangwenzheng
kongdeju
lixiyuan
```
Generate a table of user information (users-table.txt)
```
guojincheng 2001 2001 /home/guojincheng
fangshuangsang 2002 2002 /home/fangshuangsang
wangxinmei 2003 2003 /home/wangxinmei
sunliang 2004 2004 /home/sunliang
liaoshengyou 2005 2005 /home/liaoshengyou
weibin 2006 2006 /home/weibin
luochunlong 2007 2007 /home/luochunlong
wuyang 2008 2008 /home/wuyang
zhengjiawen 2009 2009 /home/zhengjiawen
fangwenzheng 2010 2010 /home/fangwenzheng
kongdeju 2011 2011 /home/kongdeju
lixiyuan 2012 2012 /home/lixiyuan
shibinbin 2013 2013 /home/shibinbin
```
Generate initial password
```
slappasswd -h "{CRYPT}" -s QGWwWxf3zG
```
**change-password.sh**
```
#! /bin/bash
suffix=${suffix:="dc=cluster,dc=com"}
while read username uid gid home;do
    if [ -z "$username" ];then
        continue
    fi
    echo "dn: uid=$username,ou=People,$suffix"
    echo "changetype: modify"
    echo "replace: userPassword"
    password=$(slappasswd -h "{CRYPT}" -s "${username}@18191")
    echo "userPassword: $password"
    echo
done
shibinbin@18191
```
Change the password:
```
ldapmodify -x -D cn=root,dc=cluster,dc=com -W -f change-password.ldif
```

Create users for LDAP (**create-users.sh**)
```
#! /bin/bash
suffix=${suffix:="dc=cluster,dc=com"}
while read username uid gid home;do
    if [ -z "$username" ];then
        continue
    fi
    echo "dn: uid=$username,ou=People,$suffix"
    echo "uid: $username"
    echo "cn: $username"
    echo "objectClass: account"
    echo "objectClass: posixAccount"
    echo "objectClass: top"
    echo "objectClass: shadowAccount"
    echo "userPassword: {crypt}!!"
    echo "shadowLastChange: 17248"
    echo "shadowMax: 99999"
    echo "shadowWarning: 7"
    echo "loginShell: /bin/bash"
    echo "uidNumber: $uid"
    echo "gidNumber: $gid"
    echo "homeDirectory: $home"
    echo

    echo "dn: cn=$username,ou=Group,$suffix"
    echo "objectClass: posixGroup"
    echo "objectClass: top"
    echo "cn: $username"
    echo "userPassword: {crypt}x"
    echo "gidNumber: $gid"
    echo
done
```
Change home directory (change-home.sh)
```
#! /bin/bash
suffix=${suffix:="dc=cluster,dc=com"}
while read username uid gid home;do
    if [ -z "$username" ];then
        continue
    fi
    echo "dn: uid=$username,ou=People,$suffix"
    echo "changetype: modify"
    echo "replace: homeDirectory"
    echo "homeDirectory: /lustre/users/$username"
    echo
done
```
