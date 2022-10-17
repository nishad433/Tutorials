
TOP:=${shell pwd}

include ${TOP}/tags.mk

PATH=$(shell printenv PATH):${TOOLCHAIN_BIN}
export PATH

ifeq (${ARCH},arm32)
OBJS := ${BOOT_SRC}/boot32.o	
else
ifeq (${ARCH},arm64)
OBJS := ${BOOT_SRC}/boot64.o	\
	${C_SRC}/gpio.o		\
	${C_SRC}/mbox.o 
endif
endif

OBJS += ${C_SRC}/main.o \
	${C_SRC}/uart.o \
	${C_SRC}/printk.o \


build :
	${MAKE} -C src 
	${LD} ${OBJS} -T ${LDSCRIPT} ${LDFLAGS} -o ${TARGET}.elf
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

.PHONY: clean
clean :
	${MAKE} -C src clean
	rm -rf *.o ${TARGET}.elf ${TARGET}.bin


