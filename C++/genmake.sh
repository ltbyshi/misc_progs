#! /bin/bash

AllTargets=$(basename -a -s .cpp *.cpp | tr '\n' ' ')
if [ -f Makefile.in ];then
    Custom=$(grep '^#' Makefile.in | cut -d' ' -f2 | tr '\n' ' ')
    Targets=$(echo $AllTargets $Custom $Custom | tr ' ' '\n' | sort | uniq -u | tr '\n' ' ')
fi
{
if [ -f "LocalConfig.mk" ];then
    echo -e 'include LocalConfig.mk'
fi
echo -e 'CXX=g++'
echo -e 'CXXFLAGS=-g -Wall -std=c++11 -Wno-pmf-conversions -I./include'
echo -e 'LDFLAGS=-lm -lpthread'
echo -e 
#echo -e "BINS=bin \$(addprefix bin/,$AllTargets)\n"
echo -e "BINS=bin $AllTargets\n"
echo -e 'all: $(BINS)\n'
echo -e ".PHONY: all clean $AllTargets\n"
echo -e 'clean:\n\trm -rf bin src/*.o lib/*.a\n'
echo -e 'bin:\n\tmkdir $@\n'
for t in $AllTargets;do
    echo -e "$t: bin bin/$t\n"
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
