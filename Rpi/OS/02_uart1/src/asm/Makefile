CROSS_COMPILER ?= aarch64-linux-gnu-
CC = $(CROSS_COMPILER)gcc
LD = $(CROSS_COMPILER)ld
OBJCOPY = $(CROSS_COMPILER)objcopy

CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
LDFLAGS = -nostdlib -nostartfiles

%.o: %.s
	$(CC) $(CFLAGS) -c $^ -o $@ 

all: boot.o

.PHONY: clean
clean:
	rm -rf *.o
