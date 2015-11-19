CXX=gcc
CFLAGS=-g -Wall
LDFLAGS=-lm
BINS=bin $(addprefix bin/,asm bitfield builtin complex factorial filelock float ioctl optimize vec )

all: $(BINS)

.PHONY: all clean

clean:
	rm -rf bin

bin:
	mkdir $@

bin/asm: asm.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/bitfield: bitfield.c
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

bin/ioctl: ioctl.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/optimize: optimize.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

bin/vec: vec.c
	$(CXX) $(CFLAGS) -o $@ $? $(LDFLAGS)

