#! /bin/sh

SyncTo(){
    if [ -z "#1" ];then
        echo "Usage: SyncTo remote_host"
        return 1
    fi
    echo "Sync to $1"
rsync -t -rav \
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

#SyncTo cs3:/sdat2/KurokawaLab/binbin/Tests
#SyncTo ddbj:Tests
#SyncTo edb:Tests
#SyncTo ddbj2:Tests
#SyncTo ncrna:Projects/Tests
#SyncTo ibme:projects/tests
SyncTo gpuserver:projects/tests
#SyncTo ddbj:projects/tests
SyncTo gpudesktop:projects/tests
