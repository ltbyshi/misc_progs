CXX=gcc
CFLAGS=-std=c99 -g -Wall -D_POSIX_C_SOURCE=200112L
LDFLAGS=-lm -lrt -lpthread
BINS=bin $(addprefix bin/,alarm asm atomic bitfield brk builtin clearline complex factorial filelock float free gets ioctl jit longjmp memfrag optimize pipe poll scrub sdlaudio shm signal struct syscall truncate unbuffered vec wchar )

all: $(BINS)

.PHONY: all clean

clean:
	rm -rf bin

bin:
	mkdir $@

bin/alarm: alarm.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/asm: asm.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/atomic: atomic.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/bitfield: bitfield.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/brk: brk.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/builtin: builtin.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/clearline: clearline.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/complex: complex.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/factorial: factorial.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/filelock: filelock.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/float: float.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/free: free.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/gets: gets.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/ioctl: ioctl.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/jit: jit.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/longjmp: longjmp.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/memfrag: memfrag.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/optimize: optimize.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/pipe: pipe.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/poll: poll.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/scrub: scrub.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/sdlaudio: sdlaudio.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/shm: shm.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/signal: signal.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/struct: struct.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/syscall: syscall.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/truncate: truncate.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/unbuffered: unbuffered.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/vec: vec.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/wchar: wchar.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

