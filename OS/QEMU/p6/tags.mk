ARCH?=arm64

ifeq (${DEBUG},1)
MAKE_FLAGS:=-w
else
MAKE_FLAGS:=--no-print-directory
endif

ifeq (${ARCH},arm64)
TARGET:=kernel8
MACHINE:=raspi3
TOOLCHAIN:=aarch64
CROSS_COMPILE:=aarch64-none-elf-
endif
ifeq (${ARCH},arm32)
TARGET:=kernel
MACHINE:=vexpress-a9
TOOLCHAIN:=arm
CROSS_COMPILE:=arm-none-eabi-
endif

ifeq (${CONSOLE},UART1)
SERIAL_CONSOLE:=-serial null -serial stdio
else
CONSOLE:=UART0
SERIAL_CONSOLE:=-serial stdio -serial null
endif

ifeq (${MACHINE},raspi3)
DEFINES:=-DRASPI3 -DARM64 -DAARCH64
#mkfifo /tmp/guest.in /tmp/guest.out
#QEMU_CMD:=qemu-system-aarch64 -M ${MACHINE} -smp 4 -m 1024M  -no-reboot -nographic -serial pipe:/tmp/guest.out -kernel ${TARGET}.bin 
QEMU_CMD:=qemu-system-aarch64 -M ${MACHINE} -smp 4 -m 1024M  -no-reboot -nographic -kernel ${TARGET}.bin 
#QEMU_CMD:=qemu-system-aarch64 -M ${MACHINE} -smp 4 -m 1024M -no-reboot ${SERIAL_CONSOLE} -kernel ${TARGET}.bin 
QEMU_GDB_CMD:=qemu-system-aarch64 -M ${MACHINE} -smp 4 -m 1024M -no-reboot ${SERIAL_CONSOLE} -kernel ${TARGET}.bin -S -gdb tcp::3333
endif

ifeq (${MACHINE},vexpress-a9)
DEFINES:=-DVEXP_A9 -DARM32 -DARM
QEMU_GDB_CMD:=qemu-system-arm -M ${MACHINE} -m 512M -no-reboot -nographic -kernel ${TARGET}.bin -S -gdb tcp::3333
QEMU_CMD:=qemu-system-arm -M ${MACHINE} -m 512M -no-reboot -nographic -kernel ${TARGET}.bin
endif

TOOLCHAIN_ROOT:=/home/nishad/Documents//toolchain/
TOOLCHAIN_PATH:=${TOOLCHAIN_ROOT}/${TOOLCHAIN}
TOOLCHAIN_BIN:=${TOOLCHAIN_PATH}/bin/
PATH=$(shell printenv PATH):${TOOLCHAIN_BIN}

INCLUDES:=-I${TOP}/include/
DEFINES+=-DCONSOLE_${CONSOLE}

AS:=${CROSS_COMPILE}as
CC:=${CROSS_COMPILE}gcc
LD:=${CROSS_COMPILE}ld
OBJCOPY:=${CROSS_COMPILE}objcopy
OBJDUMP:=${CROSS_COMPILE}objdump

ifeq (${ARCH},arm64)
ASFLAGS:= -O0 -ggdb3 -g3 -march=armv8-a ${DEFINES} ${INCLUDES}
endif

ifeq (${ARCH},arm32)
ASFLAGS:= -O0 -ggdb3 -g3 -march=armv7-a ${DEFINES} ${INCLUDES}
endif

CFLAGS= -O0 -ggdb3 -g3 -Wall -MMD ${DEFINES} ${INCLUDES} -nostdlib -nostartfiles

ifeq (${ARCH},arm64)
LDFLAGS:= -L${TOOLCHAIN_PATH}/aarch64-none-elf/lib/ -lc 
LDFLAGS+= -L${TOOLCHAIN_PATH}/lib/gcc/aarch64-none-elf/11.2.1/ -lgcc
LDFLAGS+= -L${TOOLCHAIN_PATH}/aarch64-none-elf/lib/ -lm
LDSCRIPT:=link64.ld
endif

ifeq (${ARCH},arm32)
LDFLAGS:= -L${TOOLCHAIN_PATH}/arm-none-eabi/lib/ -lc
LDFLAGS+= -L${TOOLCHAIN_PATH}/lib/gcc/arm-none-eabi/12.2.0/ -lgcc
LDFLAGS+= -L${TOOLCHAIN_PATH}/arm-none-eabi/lib/ -lm
LDSCRIPT:=link32.ld
endif

export PATH
export ARCH ASFLAGS AS CC CFLAGS LDFLAGS MAKE_FLAGS MACHINE
