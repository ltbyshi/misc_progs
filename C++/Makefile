include LocalConfig.mk
CXX=g++
CXXFLAGS=-g -Wall -std=c++11 -Wno-pmf-conversions -I./include
LDFLAGS=-lm -lpthread

BINS=bin animate argparse argsort asm auto benchmark_sort bitset bloat boost_filesystem boost_proto bus constructor dataframe delete demangle encrypt entropy exception externc fclose filedes fisher fopen fpe fpointer getopt goto hdf5test ispointer malloc map metaprog mmap mprotect mystream new nullref nullstring openmp optimize popcount prime private randbin rapidjson register segfault setops setuid sharedptr signal_template sort split sse static_assert svd template thread throw typeid undefined unicode unlink unsigned unwind vector virtual2 virtual voidtype wctime 

all: $(BINS)

.PHONY: all clean animate argparse argsort asm auto benchmark_sort bitset bloat boost_filesystem boost_proto bus constructor dataframe delete demangle encrypt entropy exception externc fclose filedes fisher fopen fpe fpointer getopt goto hdf5test ispointer malloc map metaprog mmap mprotect mystream new nullref nullstring openmp optimize popcount prime private randbin rapidjson register segfault setops setuid sharedptr signal_template sort split sse static_assert svd template thread throw typeid undefined unicode unlink unsigned unwind vector virtual2 virtual voidtype wctime 

clean:
	rm -rf bin src/*.o lib/*.a

bin:
	mkdir $@

animate: bin/animate

argparse: bin/argparse

argsort: bin/argsort

asm: bin/asm

auto: bin/auto

benchmark_sort: bin/benchmark_sort

bitset: bin/bitset

bloat: bin/bloat

boost_filesystem: bin/boost_filesystem

boost_proto: bin/boost_proto

bus: bin/bus

constructor: bin/constructor

dataframe: bin/dataframe

delete: bin/delete

demangle: bin/demangle

encrypt: bin/encrypt

entropy: bin/entropy

exception: bin/exception

externc: bin/externc

fclose: bin/fclose

filedes: bin/filedes

fisher: bin/fisher

fopen: bin/fopen

fpe: bin/fpe

fpointer: bin/fpointer

getopt: bin/getopt

goto: bin/goto

hdf5test: bin/hdf5test

ispointer: bin/ispointer

malloc: bin/malloc

map: bin/map

metaprog: bin/metaprog

mmap: bin/mmap

mprotect: bin/mprotect

mystream: bin/mystream

new: bin/new

nullref: bin/nullref

nullstring: bin/nullstring

openmp: bin/openmp

optimize: bin/optimize

popcount: bin/popcount

prime: bin/prime

private: bin/private

randbin: bin/randbin

rapidjson: bin/rapidjson

register: bin/register

segfault: bin/segfault

setops: bin/setops

setuid: bin/setuid

sharedptr: bin/sharedptr

signal_template: bin/signal_template

sort: bin/sort

split: bin/split

sse: bin/sse

static_assert: bin/static_assert

svd: bin/svd

template: bin/template

thread: bin/thread

throw: bin/throw

typeid: bin/typeid

undefined: bin/undefined

unicode: bin/unicode

unlink: bin/unlink

unsigned: bin/unsigned

unwind: bin/unwind

vector: bin/vector

virtual2: bin/virtual2

virtual: bin/virtual

voidtype: bin/voidtype

wctime: bin/wctime

bin/animate: animate.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/argsort: argsort.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/asm: asm.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/auto: auto.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/benchmark_sort: benchmark_sort.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/bitset: bitset.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/bloat: bloat.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/bus: bus.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/constructor: constructor.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/dataframe: dataframe.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/delete: delete.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/demangle: demangle.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/entropy: entropy.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/exception: exception.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/externc: externc.cpp
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

bin/ispointer: ispointer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/malloc: malloc.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/map: map.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/metaprog: metaprog.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/mmap: mmap.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/mprotect: mprotect.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/mystream: mystream.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/new: new.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/nullref: nullref.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/nullstring: nullstring.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/optimize: optimize.cpp
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

bin/setops: setops.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/setuid: setuid.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/sharedptr: sharedptr.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/signal_template: signal_template.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/sort: sort.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/split: split.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/sse: sse.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/static_assert: static_assert.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/template: template.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/thread: thread.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/throw: throw.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/typeid: typeid.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/undefined: undefined.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/unicode: unicode.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/unlink: unlink.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/unsigned: unsigned.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/unwind: unwind.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/vector: vector.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/virtual: virtual.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/virtual2: virtual2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/voidtype: voidtype.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/wctime: wctime.cpp
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


BOOST_CXXFLAGS = 
ifdef BOOST_INCLUDE_PATH
	BOOST_CXXFLAGS = -I$(BOOST_INCLUDE_PATH)
endif
BOOST_LDFLAGS =
ifdef BOOST_LIBRARY_PATH
	BOOST_LDFLAGS = -L$(BOOST_LIBRARY_PATH)
endif
# boost_filesystem
bin/boost_filesystem: boost_filesystem.cpp
	$(CXX) $(CXXFLAGS) $(BOOST_CXXFLAGS) -o $@ $< $(LDFLAGS) $(BOOST_LDFLAGS) -lboost_system -lboost_filesystem

# boost_proto
bin/boost_proto: boost_proto.cpp
	$(CXX) $(CXXFLAGS) $(BOOST_CXXFLAGS) -o $@ $< $(LDFLAGS)

# openmp
bin/openmp: openmp.cpp
	$(CXX) $(CXXFLAGS) -fopenmp -o $@ $< $(LDFLAGS)

# encrypt
bin/encrypt: encrypt.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) -lcrypto

EIGEN_CXXFLAGS =
ifdef EIGEN_INCLUDE_PATH
	EIGEN_CXXFLAGS = -I$(EIGEN_INCLUDE_PATH)
endif

# svd
bin/svd: svd.cpp
	$(CXX) $(CXXFLAGS) $(EIGEN_CXXFLAGS) -o $@ $< $(LDFLAGS)

