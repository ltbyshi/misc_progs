#! /bin/bash


# Install OpenLDAP
# Reference: http://quark.humbug.org.au/publications/ldap/intro_ldap/index.htm
# Reference (CentOS 7): https://www.server-world.info/en/note?os=CentOS_7&p=openldap
sudo yum install openldap-servers  openldap openldap-clients nss_ldap
# Copy configuration file
/etc/openldap/slapd.conf
# Download migration tool from: http://www.padl.com/OSS/MigrationTools.html
export LDAP_HOST='master.centos7-2.vbox'
export LDAP_BASEDN='dc=centos7-2,dc=vbox'
export LDAPHOST='master.centos7-2.vbox'
cn=root,dc=centos7-2,dc=vbox
# export users in the passwd file
grep <filter> /etc/passwd > passwd.user
grep <filter> /etc/group > group.user
./migrate_passwd.pl passwd.user passwd.ldif
./migrate_group.pl group.user group.ldif
# Add the users to LDAP
ldapadd -x -D cn=root,dc=centos7-2,dc=vbox -W -f passwd.ldif
ldapadd -x -D cn=root,dc=centos7-2,dc=vbox -W -f group.ldif

# On the clients, install
yum install openldap openldap-clients nss_ldap

# Edit /etc/openldap/ldap.conf
# BASE    dc=centos7-2,dc=vbox
# URI     ldap://master.centos7-2.vbox

# Refernces: https://wiki.archlinux.org/index.php/LDAP_authentication#NSS_Configuration
# Edit /etc/nssswitch.conf
# passwd:     files ldap
# shadow:     files ldap
# group:      files ldap

# Edit /etc/nslcd.conf
# uri ldap://master.centos7-2.vbox
# base dc=centos7-2,dc=vbox

# Restart nslcd
systemctl restart nslcd.service

# Reference: https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/6/html/Deployment_Guide/ch-Configuring_Authentication.html
# This will update /etc/pam.d/system-auth automatically
authconfig --enableldap --ldapserver=ldap://master.centos7-2.vbox \
    --ldapbasedn='dc=centos7-2,dc=vbox' \
    --enableldapauth --enablemkhomedir --update

# search
ldapsearch -x '(uid=user1)'

# id
id user1

# Replication
# Reference: https://www.server-world.info/en/note?os=CentOS_7&p=openldap&f=5

# Install LDAP on gene
sudo yum install openldap openldap-servers openldap-clients nss_ldap
# Install LDAP on compute nodes

# set password by binding to the RootDN (requires manager password)
# Reference: https://www.digitalocean.com/community/tutorials/how-to-change-account-passwords-on-an-openldap-server
ldappasswd -x -H ldap:/// -D 'cn=root,dc=centos7-2,dc=vbox' -W -s <new password> 'uid=user1,ou=People,dc=centos7-2,dc=vbox'
# Change user's own password
ldappasswd -x -D 'uid=user1,ou=People,dc=centos7-2,dc=vbox' -W -S

# Install phpldapadmin: https://www.server-world.info/en/note?os=CentOS_7&p=openldap&f=7
# Login with root user: cn=root,dc=centos7-2,dc=vbox
# Or login with normal user: uid=user1,ou=People,dc=centos7-2,dc=vbox

# LDAP Multi-Master replication
# Refernce: https://www.server-world.info/en/note?os=CentOS_7&p=openldap&f=6
# Add mod_syncprov.ldif
ldapadd -Y EXTERNAL -H ldapi:/// -f mod_syncprov.ldif
# Add syncprov.ldif
ldapadd -Y EXTERNAL -H ldapi:/// -f syncprov.ldif
# Add provider information for each server
