

CROSS_COMPILER ?= aarch64-linux-gnu-
CC = $(CROSS_COMPILER)gcc
LD = $(CROSS_COMPILER)ld
OBJCOPY = $(CROSS_COMPILER)objcopy

CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
LDFLAGS = -nostdlib -nostartfiles

kernel = kernel8

OBJS := boot.o main.o

%.o: %.s
	$(CC) $(CFLAGS) -c $^ -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

all: clean $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -T link.lds -o $(kernel).elf
	$(OBJCOPY) -O binary $(kernel).elf $(kernel).img

.PHONY: clean
clean:
	rm -rf $(kernel).elf $(kernel).img *.o
