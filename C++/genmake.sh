#! /bin/bash

AllTargets=$(basename -a -s .cpp *.cpp | tr '\n' ' ')
if [ -f Makefile.in ];then
    Custom=$(grep '^#' Makefile.in | cut -d' ' -f2 | tr '\n' ' ')
    Targets=$(echo $AllTargets $Custom $Custom | tr ' ' '\n' | sort | uniq -u | tr '\n' ' ')
fi
{
echo -e 'CXX=g++'
echo -e 'CFLAGS=-g -Wall -std=c++11 -Wno-pmf-conversions'
echo -e 'LDFLAGS=-lm -lpthread'
echo -e 
echo -e "BINS=bin \$(addprefix bin/,$AllTargets)\n"
echo -e 'all: $(BINS)\n'
echo -e '.PHONY: all clean\n'
echo -e 'clean:\n\trm -rf bin\n'
echo -e 'bin:\n\tmkdir $@\n'
for t in $Targets;do
    echo "bin/$t: $t.cpp"
    echo -e '\t$(CXX) $(CFLAGS) -o $@ << $(LDFLAGS)\n'
done
if [ -f Makefile.in ];then
    echo -e '#### custom rules'
    cat Makefile.in
fi
} > Makefile
