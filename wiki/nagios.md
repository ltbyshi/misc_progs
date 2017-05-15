## Install Nagios Core on the monitor server
### Install Nagios Core
```bash
./configure --with-command-group=nagcmd
make
make install
make install-init
make install-config
make install-commandmode
make install-webconf

cp -R contrib/eventhandlers/ /usr/local/nagios/libexec/
chown -R nagios:nagios /usr/local/nagios/libexec/eventhandlers
```
### Create admin user
```bash
htpasswd –c /usr/local/nagios/etc/htpasswd.users nagiosadmin
```

### Edit Apache configuration
Edit `/etc/httpd/conf/nagios.conf`. Only allow connections from local network.

### Enable server definition
Edit `/usr/local/nagios/etc/nagios.cfg`. Uncomment the lines:
```
cfg_dir=/usr/local/nagios/etc/servers
```

### Add extra commands
Edit `/usr/local/nagios/etc/objects/commands.cfg`
```
define command{
        command_name    check_nrpe
        command_line    $USER1$/check_nrpe -H $HOSTADDRESS$ -c $ARG1$
}

define command{
        command_name    check_http_url
        command_line    $USER1$/check_http -I $HOSTADDRESS$ -u $ARG1$
        }

define command{
        command_name    check_ssh_port
        command_line    $USER1$/check_ssh -p $ARG1$ $HOSTADDRESS$
        }

define command{
        command_name    check_mount
        command_line    $USER1$/check_disk -u GB -w $ARG1$ -c $ARG2$ -M $ARG3$
        }

define command{
        command_name    check_dns
        command_line    $USER1$/check_dns -H $ARG1$ -s $ARG2$
        }

define command{
        command_name    check_proxy
        command_line    $USER1$/check_disk -I $ARG1$ -p $ARG2$ -u $ARG3$
        }

define command{
        command_name    check_zombie_procs
        command_line    $USER1$/check_zombie_procs -w 5 -c 10 -s Z
        }
```
### Add basic services for a host
Edit `/usr/local/nagios/etc/servers/myhost.cfg`
```
define host{
	use	linux-server
	host_name	myhost
	address	127.0.0.1
}

define service{
	use	generic-service
	host_name	myhost
	service_description	CPU Load
	check_command	check_nrpe!check_load
}

define service{
	use	generic-service
	host_name	myhost
	service_description	Current Users
	check_command	check_nrpe!check_users
}


define service{
	use	generic-service
	host_name	myhost
	service_description	Total processes
	check_command	check_nrpe!check_total_procs
}

define service{
	use	generic-service
	host_name	myhost
	service_description	Zombie processes
	check_command	check_nrpe!check_zombie_procs
}

define service{
        use                             generic-service         ; Name of service template to use
        host_name                       myhost
        service_description             Root Partition
	check_command			check_nrpe!check_root
        }

define service{
        use                             generic-service         ; Name of service template to use
        host_name                       myhost
        service_description             Swap Usage
	    check_command			check_nrpe!check_swap
        }

define service{
        use                             generic-service         ; Name of service template to use
        host_name                       myhost
        service_description             Home Partition
        check_command                   check_nrpe!check_home
        }

define service{
        use                             generic-service         ; Name of service template to use
        host_name                       myhost
        service_description             SSH
	    check_command			check_ssh
        }

```
## Install Nagios plugins on the servers

### Create user and group for nagios
```bash
groupadd -g 117 nagios
useradd -g 117 -u 117 nagios
```

### Install prerequisites
```bash
yum groupinstall 'Development Tools'
yum install openssl-devel
```
### Install Nagios plugins
```bash
cd nagios-plugins-2.1.2
./configure --with-nagios-user=nagios --with-nagios-group=nagios
make
make install
```
### Install Nagios NRPE daemon (using SysV init script for CentOS 6)
```bash
./configure --with-init-type=sysv
make all
make install
make install-config
make install-init
chkconfig --add nrpe
```
For CentOS 7, change configure to:
```bash
./configure --with-init-type=systemd
systemctl enable nrpe
```
### Edit Nagios NRPE configuration file
Edit `/usr/local/nagios/etc/nrpe.cfg`

Add monitor servers (comma-separated list):
```
allowed_hosts=127.0.0.1
```
Basic commands:
```
command[check_users]=/usr/local/nagios/libexec/check_users -w 5 -c 10
command[check_load]=/usr/local/nagios/libexec/check_load -w 15,10,5 -c 30,25,20
command[check_root]=/usr/local/nagios/libexec/check_disk -w 10% -c 5% -M /
command[check_home]=/usr/local/nagios/libexec/check_disk -w 10% -c 5% -M /home
command[check_zombie_procs]=/usr/local/nagios/libexec/check_procs -w 5 -c 10 -s Z
command[check_total_procs]=/usr/local/nagios/libexec/check_procs -w 150 -c 200
command[check_swap]=/usr/local/nagios/libexec/check_swap -w 20% -c 10%
```
### Enable TCP port 5666
```
iptables -I INPUT -p tcp --dport 5666 -j ACCEPT
```
### Start Nagnis NRPE
```bash
service nrpe start
```
