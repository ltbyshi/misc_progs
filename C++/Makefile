include LocalConfig.mk
CXX=g++
CXXFLAGS=-g -Wall -std=c++11 -Wno-pmf-conversions -I./include
LDFLAGS=-lm -lpthread

BINS=bin animate argparse argsort asm auto benchmark_sort bitset bloat boost_filesystem boost_proto bus constructor dataframe delete demangle encrypt entropy exception externc fclose filedes fisher fopen fpe fpointer getopt goto hdf5test ispointer malloc map metaprog mmap mprotect mystream new nullref nullstring openmp optimize popcount prime private randbin rapidjson register segfault setops setuid sharedptr shuf_bed signal_template sort split sse static_assert svd template thread throw typeid undefined unicode unlink unsigned unwind vector virtual2 virtual voidtype wctime 

all: $(BINS)

.PHONY: all clean animate argparse argsort asm auto benchmark_sort bitset bloat boost_filesystem boost_proto bus constructor dataframe delete demangle encrypt entropy exception externc fclose filedes fisher fopen fpe fpointer getopt goto hdf5test ispointer malloc map metaprog mmap mprotect mystream new nullref nullstring openmp optimize popcount prime private randbin rapidjson register segfault setops setuid sharedptr shuf_bed signal_template sort split sse static_assert svd template thread throw typeid undefined unicode unlink unsigned unwind vector virtual2 virtual voidtype wctime 

clean:
	rm -rf bin src/*.o lib/*.a

bin:
	mkdir $@

animate: bin bin/animate

argparse: bin bin/argparse

argsort: bin bin/argsort

asm: bin bin/asm

auto: bin bin/auto

benchmark_sort: bin bin/benchmark_sort

bitset: bin bin/bitset

bloat: bin bin/bloat

boost_filesystem: bin bin/boost_filesystem

boost_proto: bin bin/boost_proto

bus: bin bin/bus

constructor: bin bin/constructor

dataframe: bin bin/dataframe

delete: bin bin/delete

demangle: bin bin/demangle

encrypt: bin bin/encrypt

entropy: bin bin/entropy

exception: bin bin/exception

externc: bin bin/externc

fclose: bin bin/fclose

filedes: bin bin/filedes

fisher: bin bin/fisher

fopen: bin bin/fopen

fpe: bin bin/fpe

fpointer: bin bin/fpointer

getopt: bin bin/getopt

goto: bin bin/goto

hdf5test: bin bin/hdf5test

ispointer: bin bin/ispointer

malloc: bin bin/malloc

map: bin bin/map

metaprog: bin bin/metaprog

mmap: bin bin/mmap

mprotect: bin bin/mprotect

mystream: bin bin/mystream

new: bin bin/new

nullref: bin bin/nullref

nullstring: bin bin/nullstring

openmp: bin bin/openmp

optimize: bin bin/optimize

popcount: bin bin/popcount

prime: bin bin/prime

private: bin bin/private

randbin: bin bin/randbin

rapidjson: bin bin/rapidjson

register: bin bin/register

segfault: bin bin/segfault

setops: bin bin/setops

setuid: bin bin/setuid

sharedptr: bin bin/sharedptr

shuf_bed: bin bin/shuf_bed

signal_template: bin bin/signal_template

sort: bin bin/sort

split: bin bin/split

sse: bin bin/sse

static_assert: bin bin/static_assert

svd: bin bin/svd

template: bin bin/template

thread: bin bin/thread

throw: bin bin/throw

typeid: bin bin/typeid

undefined: bin bin/undefined

unicode: bin bin/unicode

unlink: bin bin/unlink

unsigned: bin bin/unsigned

unwind: bin bin/unwind

vector: bin bin/vector

virtual2: bin bin/virtual2

virtual: bin bin/virtual

voidtype: bin bin/voidtype

wctime: bin bin/wctime

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

bin/shuf_bed: shuf_bed.cpp
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

