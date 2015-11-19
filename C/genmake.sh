#! /bin/bash


Targets=$(basename -a -s .c *.c | tr '\n' ' ')
echo -e 'CXX=gcc' > Makefile
echo -e 'CFLAGS=-g -Wall' >> Makefile
echo -e 'LDFLAGS=-lm' >> Makefile
echo -e "BINS=bin \$(addprefix bin/,$Targets)\n" >> Makefile
echo -e 'all: $(BINS)\n' >> Makefile
echo -e '.PHONY: all clean\n' >> Makefile
echo -e 'clean:\n\trm -rf bin\n' >> Makefile
echo -e 'bin:\n\tmkdir $@\n' >> Makefile
for t in $Targets;do
    echo "bin/$t: $t.c" >> Makefile
    echo -e '\t$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)\n' >> Makefile
done

