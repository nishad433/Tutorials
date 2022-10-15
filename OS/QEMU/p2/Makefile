ARCH?=arm32


TARGET:=kernel

ifeq (${ARCH},arm32)
CROSS_COMPILE:=arm-none-eabi-
LDSCRIPT:=link32.ld
ASFLAGS:= -g -march=armv7-a

QEMU_GDB_CMD:=qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -kernel ${TARGET}.bin -S -gdb tcp::3333 
QEMU_CMD:=qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -kernel ${TARGET}.bin -d in_asm

OBJS:=boot32.o

else
ifeq (${ARCH},arm64)
CROSS_COMPILE:=aarch64-linux-gnu-
LDSCRIPT:=link64.ld
ASFLAGS:= -g -march=armv8-a

QEMU_CMD:=qemu-system-aarch64 -M raspi3 -no-reboot -nographic -kernel ${TARGET}.bin -d in_asm
QEMU_GDB_CMD:=qemu-system-aarch64 -M raspi3 -no-reboot -nographic -kernel ${TARGET}.bin -S -gdb tcp::3333

OBJS:=boot64.o

endif
endif

AS:=${CROSS_COMPILE}as
CC:=${CROSS_COMPILE}gcc
LD:=${CROSS_COMPILE}ld
OBJCOPY:=${CROSS_COMPILE}objcopy


CFLAGS:= -g
LDFLAGS:= -nostdlib -nostartfiles 
#LDFLAGS = -nostdlib -nostartfiles -lgcc -L./lib/

OBJS+=main.o

build : ${OBJS}
	${LD} ${LDFLAGS} -T ${LDSCRIPT} $^ -o ${TARGET}.elf
	${OBJCOPY} -O binary ${TARGET}.elf ${TARGET}.bin

%.o : %.S
	${CC} ${ASFLAGS} -c $< -o $@

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@


run-gdb :
#	qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -monitor telnet:127.0.0.1:1234,server,nowait -kernel ${TARGET}.bin
#	qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -S -gdb tcp::3333 -kernel ${TARGET}.bin
#	qemu-system-aarch64 -M raspi3 -kernel ${TARGET}.bin -d in_asm
	${QEMU_GDB_CMD}

run :
	${QEMU_CMD}

clean :
	rm -rf *.o ${TARGET}.elf ${TARGET}.bin


