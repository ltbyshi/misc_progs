#! /bin/bash

AllTargets=$(for f in *.cpp;do basename $f .cpp;done | tr '\n' ' ')
if [ -f Makefile.in ];then
    Custom=$(grep '^#' Makefile.in | cut -d' ' -f2 | tr '\n' ' ')
    Targets=$(echo $AllTargets $Custom $Custom | tr ' ' '\n' | sort | uniq -u | tr '\n' ' ')
fi
{
if [ -f "LocalConfig.mk" ];then
    echo -e 'include LocalConfig.mk'
fi
echo -e 'AR=ar'
echo -e 'CXX=mpic++'
echo -e 'CC=mpicc'
echo -e 'MPICXX=mpic++'
echo -e 'MPICC=mpicc'
echo -e 'CXXFLAGS=-g -Wall -Wno-pmf-conversions -I./include'
echo -e 'LDFLAGS=-L./lib -lm -lpthread'
echo -e 
#echo -e "BINS=bin \$(addprefix bin/,$AllTargets)\n"
echo -e "BINS=bin $AllTargets\n"
echo -e 'all: $(BINS)\n'
echo -e ".PHONY: all clean $AllTargets\n"
echo -e 'clean:\n\trm -rf bin src/*.o lib/*.a\n'
echo -e 'bin:\n\tmkdir $@\n'
echo -e 'lib:\n\tmkdir $@\n'
for t in $Targets;do
    echo -e "$t: bin/$t\n"
done
for t in $Targets;do
    echo "bin/$t: $t.cpp"
    echo -e '\t$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)\n'
done
if [ -f Makefile.in ];then
    echo -e '#### custom rules'
    cat Makefile.in
fi
} > Makefile
