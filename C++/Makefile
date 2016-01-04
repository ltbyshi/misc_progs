CXX=g++
CFLAGS=-g -Wall -std=c++11 -Wno-pmf-conversions
LDFLAGS=-lm -lpthread
BINS=bin $(addprefix bin/,argparse asm auto bitset bloat bus constructor delete entropy fclose filedes fisher fopen fpe fpointer getopt goto map metaprog mmap mprotect new nullref nullstring popcount prime private randbin register segfault setuid sort sse template thread throw typeid unlink unsigned vector virtual voidtype )

all: $(BINS)

.PHONY: all clean

clean:
	rm -rf bin

bin:
	mkdir $@

bin/argparse: argparse.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/asm: asm.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/auto: auto.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/bitset: bitset.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/bloat: bloat.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/bus: bus.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/constructor: constructor.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/delete: delete.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/entropy: entropy.cpp
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

bin/fpointer: fpointer.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/getopt: getopt.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/goto: goto.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/map: map.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/metaprog: metaprog.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/mmap: mmap.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/mprotect: mprotect.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/new: new.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/nullref: nullref.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/nullstring: nullstring.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/popcount: popcount.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/prime: prime.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/private: private.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/randbin: randbin.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/register: register.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/segfault: segfault.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/setuid: setuid.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/sort: sort.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/sse: sse.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/template: template.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/thread: thread.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/throw: throw.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/typeid: typeid.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/unlink: unlink.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/unsigned: unsigned.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/vector: vector.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/virtual: virtual.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/voidtype: voidtype.cpp
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

