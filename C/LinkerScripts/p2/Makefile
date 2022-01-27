EXE=test
CROSS_COMPILE?=
CC=$(CROSS_COMPILE)-gcc
LD=$(CROSS_COMPILE)-ld
CFLAGS?=
LDFLAGS?=--verbose


%.o: %.c
	$(CC) -c $(CFLAGS) $^

all: main.o ncc.o
	$(LD) -T script.ld $^ -o $(EXE) $(LDFLAGS)

clean:
	rm -rf *.o
	rm -rf $(EXE)
