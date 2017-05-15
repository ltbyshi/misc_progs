## DEseq2 Link
Bioconductor link: (http://www.bioconductor.org/packages/release/bioc/html/DESeq2.html)

### Login on to login01 (IBM-E)
```bash
ssh login01
```

### Install DESeq2
```bash
unset LD_LIBRARY_PATH
unset LIBRARY_PATH
unset CPATH
# set installation directory for DESeq2
INSTALL_PATH=/dev/shm/shibinbin/DESeq2
# use a GCC version that support C++11
export PATH=/Share/home/shibinbin/pkgs/gcc/4.9.4/bin:$PATH
export LD_LIBRARY_PATH=/Share/home/shibinbin/apps/lib:/Share/home/shibinbin/pkgs/gcc/4.9.4/lib64
export R_LIBS_USER=$INSTALL_PATH
mkdir -p $INSTALL_PATH
export https_proxy=http://172.235.81.3:15612
export http_proxy=http://172.235.81.3:15612
```

### Set biocondutor mirror in ~/.Rprofile
```R
options(BioC_mirror="https://mirrors.tuna.tsinghua.edu.cn/bioconductor")
```

### Start R
```R
source("https://bioconductor.org/biocLite.R")
biocLite("DESeq2")
```
