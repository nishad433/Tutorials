EXE=test
CC?=gcc
LD?=ld
CFLAGS?=
STARTUP:=-dynamic-linker /lib64/ld-linux-x86-64.so.2 /usr/lib/x86_64-linux-gnu/Scrt1.o /usr/lib/x86_64-linux-gnu/crti.o /usr/lib/x86_64-linux-gnu/crtn.o
LDFLAGS?=-lc

%.o: %.c
	$(CC) -c $(CFLAGS) $^

all: main.o
	$(LD) $(STARTUP) $^ -o $(EXE) $(LDFLAGS)

clean:
	rm -rf *.o
	rm -rf $(EXE)
