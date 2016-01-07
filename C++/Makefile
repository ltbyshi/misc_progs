CXX=g++
CXXFLAGS=-g -Wall -std=c++11 -Wno-pmf-conversions
LDFLAGS=-lm -lpthread

BINS=bin $(addprefix bin/,argparse argsort asm auto bitset bloat bus constructor delete entropy exception fclose filedes fisher fopen fpe fpointer getopt goto hdf5test map metaprog mmap mprotect new nullref nullstring popcount prime private randbin register segfault setuid sort sse template thread throw typeid unlink unsigned vector virtual voidtype )

all: $(BINS)

.PHONY: all clean

clean:
	rm -rf bin

bin:
	mkdir $@

bin/argsort: argsort.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/asm: asm.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/auto: auto.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/bitset: bitset.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/bloat: bloat.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/bus: bus.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/constructor: constructor.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/delete: delete.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/entropy: entropy.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/exception: exception.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/fclose: fclose.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/filedes: filedes.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/fisher: fisher.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/fopen: fopen.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/fpe: fpe.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/fpointer: fpointer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/getopt: getopt.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/goto: goto.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/map: map.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/metaprog: metaprog.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/mmap: mmap.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/mprotect: mprotect.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/new: new.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/nullref: nullref.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/nullstring: nullstring.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/popcount: popcount.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/prime: prime.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/private: private.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/randbin: randbin.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/register: register.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/segfault: segfault.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/setuid: setuid.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/sort: sort.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/sse: sse.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/template: template.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/thread: thread.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/throw: throw.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/typeid: typeid.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/unlink: unlink.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/unsigned: unsigned.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/vector: vector.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/virtual: virtual.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/voidtype: voidtype.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

#### custom rules
# hdf5test
HDF5TESt_LDFLAGS = $(LDFLAGS)
ifdef HDF5_LIBRARY_PATH
  HDF5TEST_LDFLAGS += -L$(HDF5_LIBRARY_PATH)
endif
HDF5TEST_LDFLAGS += -lhdf5_hl_cpp -lhdf5_cpp -lhdf5_hl -lhdf5
HDF5TEST_CXXFLAGS = $(CXXFLAGS)
ifdef HDF5_INCLUDE_PATH
  HDF5TEST_CXXFLAGS += -I$(HDF5_INCLUDE_PATH)
endif
bin/hdf5test: hdf5test.cpp
	$(CXX) $(HDF5TEST_CXXFLAGS) -o $@ $< $(HDF5TEST_LDFLAGS)

# argparse
bin/argparse: argparse.cpp argparse.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

