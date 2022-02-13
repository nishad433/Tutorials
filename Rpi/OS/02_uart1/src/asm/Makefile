#CROSS_COMPILER ?= aarch64-linux-gnu-
#CC = $(CROSS_COMPILER)gcc
#LD = $(CROSS_COMPILER)ld
#OBJCOPY = $(CROSS_COMPILER)objcopy

#CFLAGS = -g -Wall -ffreestanding
#LDFLAGS = -nostdlib -nostartfiles

%.o: %.s
	$(CC) $(CFLAGS) -c $^ -o $@ 

all: boot.o

.PHONY: clean
clean:
	rm -rf *.o
