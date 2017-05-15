## 账户管理
账户管理软件是OpenLDAP ([https://www.openldap.org/](https://www.openldap.org/))。
跟Linux自带的useradd，userdel等管理工具不同，OpenLDAP的账户不是存储在每台机器的`/etc/passwd`文件里，而是在一台主节点上（目前是master节点）。
具体路径是在`/var/lib/ldap`目录下。
主节点master上安装了一个网页版的管理软件phpLDAPadmin，在10.0.0.0/24内网内可以通过[http://10.0.0.54/ldapadmin](http://10.0.0.54/ldapadmin)来访问。

打开phpLDAPadmin后，通过管理员的DN `cn=root,dc=cluster,dc=com`和密码登录。
在登录后的界面中，`People`和`Group`两项分别代表了用户和组。

## 命令行的账户管理方式
### 添加新账户
新建一个文件（add-user.ldif)，输入以下内容：
```
dn: uid=${user},ou=People,dc=cluster,dc=com
uid: ${user}
cn: ${user}
objectClass: account
objectClass: posixAccount
objectClass: top
objectClass: shadowAccount
userPassword: {crypt}!!
shadowLastChange: 17248
shadowMax: 99999
shadowWarning: 7
loginShell: /bin/bash
uidNumber: ${uid}
gidNumber: ${gid}
homeDirectory: /lustre/users/${user}
```
之后以下运行命令来添加账户（需要输入管理员密码）：
```
ldapadd -x -D cn=root,dc=cluster,dc=com -W -f add-user.ldif
```
**注意**：把`${user}`、`${uid}`和`${uid}`分别改成对应的用户名（字符串），用户ID号（数字）和组的ID号（数字）。例如BioIT的gid是2000。初始密码为空，需要后续更改密码。

### 删除账户
通过以下命令删除账户（需要输入管理员密码）：
```
ldapdelete -x 'uid=${user},ou=People,dc=cluster,dc=com' -D cn=root,dc=cluster,dc=com -W
```
**注意**：把`${user}`替换为用户名（字符串）。

### 删除组
```
ldapdelete -x 'cn=${group},ou=Group,dc=cluster,dc=com' -D cn=root,dc=cluster,dc=com -W
```
**注意**：把`${group}`替换为用户组名（字符串）。

### 生成密码
运行以下命令将会产生一个加密后的新密码，可以用于替换上述的`add-user.ldif`文件中的`userPassword`后面的内容：
```
slappasswd -h '{CRYPT}'
```
该命令会产生一个类似`{CRYPT}4KFF5pJGCV26Q`的密码。

### 修改密码
用户可以直接在任何一台服务器上通过`passwd`命令修改自己的密码。

管理员可以通过以下方式来修改其他用户的密码。首先创建一个文件(`change-password.ldif`)
```
dn: uid=${user},ou=People,dc=cluster,dc=com
changetype: modify
replace: userPassword
userPassword: ${password}
```
然后运行以下命令（需要管理员密码）：
```
ldapmodify -x -D cn=root,dc=cluster,dc=com -W -f change-password.ldif
```
**注意**：把`${user}`和`${password}`分别替换为用户名（字符串）和上述`slappasswd`命令生成的密码。

### 添加用户组
首先新建一个文件(`add-group.ldif`)，加入以下内容：
```
dn: cn=${group},ou=Group,dc=cluster,dc=com
objectClass: posixGroup
objectClass: top
cn: ${group}
userPassword: {crypt}x
gidNumber: ${gid}
```
```
ldapadd -x -D cn=root,dc=cluster,dc=com -W -f add-group.ldif
```
**注意**：把`${group}`和`${gid}`分别替换为用户组名（字符串）和组ID（数字）。

### 修改用户组
首先新建一个文件(`change-group.ldif`)，加入以下内容：
```
dn: uid=${user},ou=People,dc=cluster,dc=com
changetype: modify"
replace: gidNumber"
gidNumber: ${gid}
```
然后运行以下命令（需要管理员密码）：
```
ldapmodify -x -D cn=root,dc=cluster,dc=com -W -f change-group.ldif
```
**注意**：把`${group}`和`${gid}`分别替换为新的用户组名（字符串）和组ID（数字）。

### 修改用户信息
建议通过phpLDAPadmin网页版来方便地修改每个用户的信息，命令行方式操作类似，通过`ldapmodify`这个命令。

例如，修改用户的home目录，就需要首先新建文件(`change-home.ldif`)
```
dn: uid=${user},ou=People,dc=cluster,dc=com
changetype: modify
replace: homeDirectory
homeDirectory: ${home}
```
**注意**：把`${user}`和`${home}`分别替换成用户名（字符串）和新的home目录的路径。

然后运行以下命令（需要管理员密码）：
```
ldapmodify -x -D cn=root,dc=cluster,dc=com -W -f change-home.ldif
```
### 查看用户信息
可以通过`ldapsearch`这个命令来查看用户或者用户组的信息。

例如，搜索一个用户：
```
ldapsearch -x '(uid=${user})'
```
将会显示类似下面的内容：
```
# extended LDIF
#
# LDAPv3
# base <dc=cluster,dc=com> (default) with scope subtree
# filter: (uid=shibinbin)
# requesting: ALL
#

# ${user}, People, cluster.com
dn: uid=${user},ou=People,dc=cluster,dc=com
uid: ${user}
cn: ${user}
objectClass: account
objectClass: posixAccount
objectClass: top
objectClass: shadowAccount
shadowMax: 99999
shadowWarning: 7
loginShell: /bin/bash
uidNumber: ${uid}
homeDirectory: /lustre/users/${user}
gidNumber: ${gid}

# search result
search: 2
result: 0 Success

# numResponses: 2
# numEntries: 1
```

## 寻找最大的uid
```bash
ldapsearch -x '(objectClass=*)' | grep uidNumber | awk '{print $2}' | sort -k1,1n
```

## 备份和恢复
导出整个数据库到一个LDIF文件中：
```
slapcat -b 'dc=cluster,dc=com' > backup.ldif
```
从空的数据库恢复之前的备份文件：
```
slapadd -c -l backup.ldif
```

## 新的root密码
jingyun2017
