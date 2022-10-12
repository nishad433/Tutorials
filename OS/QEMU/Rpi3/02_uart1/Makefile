

export CROSS_COMPILER ?= aarch64-linux-gnu-
export CC = $(CROSS_COMPILER)gcc
export LD = $(CROSS_COMPILER)ld
export OBJCOPY = $(CROSS_COMPILER)objcopy

export CFLAGS = -g -Wall -ffreestanding
export LDFLAGS = -nostdlib -nostartfiles
export TOP := $(shell pwd)

kernel = kernel8

OBJS := $(TOP)/src/asm/boot.o \
	$(TOP)/src/c/gpio.o   \
	$(TOP)/src/c/uart.o   \
	$(TOP)/src/c/printk.o   \
	$(TOP)/src/c/main.o

all: subdirs

subdirs:
	$(MAKE) -C src/ all
	$(LD) $(LDFLAGS) $(OBJS) -T link.ld -o $(kernel).elf
	$(OBJCOPY) -O binary $(kernel).elf $(kernel).img

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio

.PHONY: clean
clean:
	make -C src/ clean
	rm -rf $(kernel).elf $(kernel).img 
