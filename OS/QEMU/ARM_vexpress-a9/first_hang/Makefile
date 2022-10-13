
TARGET:=first-hang
CROSS_COMPILE?=arm-none-eabi-
AS:=${CROSS_COMPILE}as
CC:=${CROSS_COMPILE}gcc
LD:=${CROSS_COMPILE}ld
OBJCOPY:=${CROSS_COMPILE}objcopy

LDFLAGS = -nostdlib -nostartfiles

%.o:	%.S
	${AS} -o $@ $<

all: startup.o
	$(LD) $(LDFLAGS) $^ -T link.ld -o $(TARGET).elf
	${OBJCOPY} -O binary ${TARGET}.elf ${TARGET}.bin

run:
	qemu-system-arm -M vexpress-a9 -m 32M -no-reboot -nographic -monitor telnet:127.0.0.1:1234,server,nowait -kernel ${TARGET}.bin

clean:
	rm -rf *.o
	rm -rf ${TARGET}.*
