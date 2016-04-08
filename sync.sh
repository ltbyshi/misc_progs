#! /bin/sh

SSH_CMD="ssh -o 'ProxyCommand=/usr/bin/nc --proxy-type socks5 --proxy=127.0.0.1:1080 %h %p'"

SyncTo(){
    if [ -z "#1" ];then
        echo "Usage: SyncTo remote_host"
        return 1
    fi
rsync -t -e "$SSH_CMD" -rAv \
  --exclude='.git/*' \
  --exclude='*.o' \
  --exclude='C/bin/' --exclude='C/lib/' --exclude="C/LocalConfig.mk" --exclude="C/Makefile" \
  --exclude='C++/bin/' --exclude='C++/lib/' --exclude="C++/LocalConfig.mk" --exclude="C++/Makefile" \
  --exclude='mpi/bin/' --exclude='mpi/lib/' --exclude='mpi/LocalConfig.mk' --exclude='mpi/Makefile' \
  --exclude='*.pyc' \
  --exclude='parallel/' \
  --exclude='*.kate-swp' \
  --exclude='*.swp' \
  --delete \
  ./ $1
}

SyncTo binbin@192.168.1.53:/sdat2/KurokawaLab/binbin/Tests
SyncTo shibinbin@gw.ddbj.nig.ac.jp:Tests
