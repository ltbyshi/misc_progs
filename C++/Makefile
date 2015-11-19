CXX=g++
CFLAGS=-g -Wall -std=c++11
LDFLAGS=-lm
BINS=bin $(addprefix bin/,asm auto bitset bus constructor delete fclose filedes fisher fopen fpe getopt goto mmap mprotect new nullref popcount prime register segfault setuid sse template throw typeid unlink unsigned )

all: $(BINS)

.PHONY: all clean

clean:
	rm -rf bin

bin:
	mkdir $@

bin/asm: asm.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/auto: auto.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/bitset: bitset.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/bus: bus.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/constructor: constructor.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/delete: delete.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/fclose: fclose.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/filedes: filedes.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/fisher: fisher.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/fopen: fopen.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/fpe: fpe.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/getopt: getopt.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/goto: goto.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/mmap: mmap.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/mprotect: mprotect.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/new: new.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/nullref: nullref.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/popcount: popcount.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/prime: prime.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/register: register.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/segfault: segfault.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/setuid: setuid.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/sse: sse.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/template: template.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/throw: throw.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/typeid: typeid.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/unlink: unlink.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/unsigned: unsigned.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

