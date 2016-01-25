include LocalConfig.mk
CXX=g++
CXXFLAGS=-g -Wall -std=c++11 -Wno-pmf-conversions -I./include
LDFLAGS=-lm -lpthread

BINS=bin animate argparse argsort asm auto bitset bloat bus constructor dataframe delete demangle entropy exception fclose filedes fisher fopen fpe fpointer getopt goto hdf5test ispointer map metaprog mmap mprotect mystream new nullref nullstring popcount prime private randbin rapidjson register segfault setuid sort split sse static_assert template thread throw typeid unlink unsigned unwind vector virtual2 virtual voidtype 

all: $(BINS)

.PHONY: all clean animate argparse argsort asm auto bitset bloat bus constructor dataframe delete demangle entropy exception fclose filedes fisher fopen fpe fpointer getopt goto hdf5test ispointer map metaprog mmap mprotect mystream new nullref nullstring popcount prime private randbin rapidjson register segfault setuid sort split sse static_assert template thread throw typeid unlink unsigned unwind vector virtual2 virtual voidtype 

clean:
	rm -rf bin

bin:
	mkdir $@

animate: bin/animate

bin/animate: animate.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

argsort: bin/argsort

bin/argsort: argsort.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

asm: bin/asm

bin/asm: asm.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

auto: bin/auto

bin/auto: auto.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bitset: bin/bitset

bin/bitset: bitset.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bloat: bin/bloat

bin/bloat: bloat.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bus: bin/bus

bin/bus: bus.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

constructor: bin/constructor

bin/constructor: constructor.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

dataframe: bin/dataframe

bin/dataframe: dataframe.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

delete: bin/delete

bin/delete: delete.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

demangle: bin/demangle

bin/demangle: demangle.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

entropy: bin/entropy

bin/entropy: entropy.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

exception: bin/exception

bin/exception: exception.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

fclose: bin/fclose

bin/fclose: fclose.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

filedes: bin/filedes

bin/filedes: filedes.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

fisher: bin/fisher

bin/fisher: fisher.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

fopen: bin/fopen

bin/fopen: fopen.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

fpe: bin/fpe

bin/fpe: fpe.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

fpointer: bin/fpointer

bin/fpointer: fpointer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

getopt: bin/getopt

bin/getopt: getopt.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

goto: bin/goto

bin/goto: goto.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

ispointer: bin/ispointer

bin/ispointer: ispointer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

map: bin/map

bin/map: map.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

metaprog: bin/metaprog

bin/metaprog: metaprog.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

mmap: bin/mmap

bin/mmap: mmap.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

mprotect: bin/mprotect

bin/mprotect: mprotect.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

mystream: bin/mystream

bin/mystream: mystream.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

new: bin/new

bin/new: new.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

nullref: bin/nullref

bin/nullref: nullref.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

nullstring: bin/nullstring

bin/nullstring: nullstring.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

popcount: bin/popcount

bin/popcount: popcount.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

prime: bin/prime

bin/prime: prime.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

private: bin/private

bin/private: private.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

randbin: bin/randbin

bin/randbin: randbin.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

register: bin/register

bin/register: register.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

segfault: bin/segfault

bin/segfault: segfault.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

setuid: bin/setuid

bin/setuid: setuid.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

sort: bin/sort

bin/sort: sort.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

split: bin/split

bin/split: split.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

sse: bin/sse

bin/sse: sse.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

static_assert: bin/static_assert

bin/static_assert: static_assert.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

template: bin/template

bin/template: template.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

thread: bin/thread

bin/thread: thread.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

throw: bin/throw

bin/throw: throw.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

typeid: bin/typeid

bin/typeid: typeid.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

unlink: bin/unlink

bin/unlink: unlink.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

unsigned: bin/unsigned

bin/unsigned: unsigned.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

unwind: bin/unwind

bin/unwind: unwind.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

vector: bin/vector

bin/vector: vector.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

virtual: bin/virtual

bin/virtual: virtual.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

virtual2: bin/virtual2

bin/virtual2: virtual2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

voidtype: bin/voidtype

bin/voidtype: voidtype.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

#### custom rules
# hdf5test
HDF5TEST_LDFLAGS = $(LDFLAGS)
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
bin/argparse: argparse.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# rapidjson
RAPIDJSON_CXXFLAGS = 
ifdef RAPIDJSON_INCLUDE_PATH
  RAPIDJSON_CXXFLAGS = -I$(RAPIDJSON_INCLUDE_PATH)
endif
bin/rapidjson: rapidjson.cpp
	$(CXX) $(CXXFLAGS) $(RAPIDJSON_CXXFLAGS) -o $@ $< $(LDFLAGS)
	
