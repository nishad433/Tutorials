EXE=test


CC?=gcc
CFLAGS?=-g -nostdlib

%.o	: %.c
	$(CC) $(CFLAGS) -c $< 

%.o	: %.S
	$(CC) $(CFLAGS) -c $< 

all	: main.o start.o
	$(CC) $(CFLAGS) $^ -o $(EXE)

clean	:
	rm -rf *.o
	rm -rf $(EXE)
