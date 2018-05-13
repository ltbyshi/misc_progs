#! /bin/bash
suffix=${suffix:?}
while read username uid gid home;do
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
