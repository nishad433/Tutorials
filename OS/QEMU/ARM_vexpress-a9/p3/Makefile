
TARGET:=kernel
CROSS_COMPILE?=arm-none-eabi-
AS:=${CROSS_COMPILE}as
CC:=${CROSS_COMPILE}gcc
LD:=${CROSS_COMPILE}ld
OBJCOPY:=${CROSS_COMPILE}objcopy

CFLAGS = -march=armv7-a -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
LDFLAGS = -nostdlib -nostartfiles

%.o:	%.S
	${CC} $(CFLAGS) -c $^ -o $@
	#${AS} -o $@ $<

all: startup.o
	$(LD) $(LDFLAGS) $^ -T link.ld -o $(TARGET).elf
	${OBJCOPY} -O binary ${TARGET}.elf ${TARGET}.bin

run-qemu:
	qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -monitor telnet:127.0.0.1:1234,server,nowait -kernel ${TARGET}.bin

run-gdb:
	qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -kernel ${TARGET}.bin -s -S

clean:
	rm -rf *.o
	rm -rf ${TARGET}.*
