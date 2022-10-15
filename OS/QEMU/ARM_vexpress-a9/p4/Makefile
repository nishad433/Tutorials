
TARGET:=kernel
CROSS_COMPILE?=arm-none-eabi-
AS:=${CROSS_COMPILE}as
CC:=${CROSS_COMPILE}gcc
LD:=${CROSS_COMPILE}ld
OBJCOPY:=${CROSS_COMPILE}objcopy

CFLAGS = -g -nostdlib -nostartfiles 
SFLAGS = -g -march=armv7-a
LDFLAGS = -nostdlib -nostartfiles -lgcc -L./lib/

%.o:	%.S
	${CC} ${SFLAGS} -c $^ -o $@

%.o:	%.c
	${CC} ${CFLAGS} -c $^ -o $@

all: startup.o main.o uart.o printk.o
	${LD} ${LDFLAGS} $^ -T link.ld -o ${TARGET}.elf
	${OBJCOPY} -O binary ${TARGET}.elf ${TARGET}.bin

run-qemu:
	qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -monitor telnet:127.0.0.1:1234,server,nowait -kernel ${TARGET}.bin

run-gdb:
	qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -S -gdb tcp::3333 -kernel ${TARGET}.bin

clean:
	rm -rf *.o
	rm -rf ${TARGET}.*
