include LocalConfig.mk
AR=ar
CXX=mpic++
CC=mpicc
CXXFLAGS=-g -Wall -std=c++11 -Wno-pmf-conversions -I./include
LDFLAGS=-L./lib -lm -lpthread

BINS=bin critical libMPIUtil lock mpish pingpong semaphore sendrecv simple 

all: $(BINS)

.PHONY: all clean critical libMPIUtil lock mpish pingpong semaphore sendrecv simple 

clean:
	rm -rf bin src/*.o lib/*.a

bin:
	mkdir $@

lib:
	mkdir $@

lock: bin/lock

mpish: bin/mpish

pingpong: bin/pingpong

semaphore: bin/semaphore

sendrecv: bin/sendrecv

simple: bin/simple

bin/lock: lock.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/mpish: mpish.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/pingpong: pingpong.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/semaphore: semaphore.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/sendrecv: sendrecv.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/simple: simple.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

#### custom rules
# libMPIUtil
libMPIUtil: lib lib/libMPIUtil.a

libMPIUtil.o: libMPIUtil.cpp
	$(MPICXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

lib/libMPIUtil.a: libMPIUtil.o
	$(AR) rcs $@ $?

# critical
critical: bin/critical lib/libMPIUtil.a

bin/critical: critical.cpp
	$(MPICXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) -lMPIUtil
