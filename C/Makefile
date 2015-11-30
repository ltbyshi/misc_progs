CXX=gcc
CFLAGS=-g -Wall
LDFLAGS=-lm -lrt -lSDL2
BINS=bin $(addprefix bin/,alarm asm atomic bitfield brk builtin complex factorial filelock float gets ioctl jit optimize scrub sdlaudio shm syscall unbuffered vec )

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

bin/complex: complex.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/factorial: factorial.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/filelock: filelock.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/float: float.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/gets: gets.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/ioctl: ioctl.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/jit: jit.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/optimize: optimize.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/scrub: scrub.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/sdlaudio: sdlaudio.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/shm: shm.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/syscall: syscall.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/unbuffered: unbuffered.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/vec: vec.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

