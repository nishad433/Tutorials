

CROSS_COMPILER ?= aarch64-linux-gnu-
CC = $(CROSS_COMPILER)gcc
LD = $(CROSS_COMPILER)ld
OBJCOPY = $(CROSS_COMPILER)objcopy

CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
LDFLAGS = -nostdlib -nostartfiles

kernel = kernel8

TOP := $(shell pwd)

OBJS := $(TOP)/src/asm/boot.o \
	$(TOP)/src/c/main.o

all: subdirs

subdirs:
	$(MAKE) -C src/ all
	$(LD) $(LDFLAGS) $(OBJS) -T link.ld -o $(kernel).elf
	$(OBJCOPY) -O binary $(kernel).elf $(kernel).img

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -d in_asm

.PHONY: clean
clean:
	make -C src/ clean
	rm -rf $(kernel).elf $(kernel).img 
