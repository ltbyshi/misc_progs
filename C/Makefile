CC=gcc
CFLAGS=-g -Wall -I./include
LDFLAGS=-lrt -lm -lpthread

BINS=bin alarm asm atomic bitfield brk builtin clearline complex factorial fasta2fastq filelock float flock free gets glibc-hack ioctl jit libjail longjmp lspy memfile memfrag optimize pipe poll scrub sdlaudio setuid shm signal struct surun syscall truncate unbuffered vec wchar 

all: $(BINS)

.PHONY: all clean alarm asm atomic bitfield brk builtin clearline complex factorial fasta2fastq filelock float flock free gets glibc-hack ioctl jit libjail longjmp lspy memfile memfrag optimize pipe poll scrub sdlaudio setuid shm signal struct surun syscall truncate unbuffered vec wchar 

clean:
	rm -rf bin src/*.o lib/*.a

bin:
	mkdir $@

lib:
	mkdir $@

alarm: bin/alarm

asm: bin/asm

atomic: bin/atomic

bitfield: bin/bitfield

brk: bin/brk

builtin: bin/builtin

clearline: bin/clearline

complex: bin/complex

factorial: bin/factorial

fasta2fastq: bin/fasta2fastq

filelock: bin/filelock

float: bin/float

flock: bin/flock

free: bin/free

gets: bin/gets

ioctl: bin/ioctl

jit: bin/jit

longjmp: bin/longjmp

memfile: bin/memfile

memfrag: bin/memfrag

optimize: bin/optimize

pipe: bin/pipe

poll: bin/poll

scrub: bin/scrub

sdlaudio: bin/sdlaudio

setuid: bin/setuid

shm: bin/shm

signal: bin/signal

struct: bin/struct

surun: bin/surun

syscall: bin/syscall

truncate: bin/truncate

unbuffered: bin/unbuffered

vec: bin/vec

wchar: bin/wchar

bin/alarm: alarm.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/asm: asm.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/atomic: atomic.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/bitfield: bitfield.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/brk: brk.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/builtin: builtin.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/clearline: clearline.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/complex: complex.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/factorial: factorial.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/fasta2fastq: fasta2fastq.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/filelock: filelock.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/float: float.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/flock: flock.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/free: free.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/gets: gets.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/ioctl: ioctl.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/jit: jit.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/longjmp: longjmp.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/memfile: memfile.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/memfrag: memfrag.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/optimize: optimize.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/pipe: pipe.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/poll: poll.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/scrub: scrub.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/sdlaudio: sdlaudio.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/setuid: setuid.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/shm: shm.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/signal: signal.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/struct: struct.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/surun: surun.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/syscall: syscall.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/truncate: truncate.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/unbuffered: unbuffered.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/vec: vec.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/wchar: wchar.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

#### custom rules
# glibc-hack
glibc-hack: lib lib/glibc-hack.so

lib/glibc-hack.so: glibc-hack.c
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $< $(LDFLAGS)

# lspy
lspy: lib lib/lspy.so

lib/lspy.so: lspy.c
	$(CC) $(CFLAGS) -D_GNU_SOURCE=1 -shared -fPIC -o $@ $< $(LDFLAGS) -ldl

# libjail
libjail: lib lib/libjail.so

lib/libjail.so: libjail.c
	$(CC) $(CFLAGS) -D_GNU_SOURCE=1 -shared -fPIC -o $@ $< $(LDFLAGS) -ldl
