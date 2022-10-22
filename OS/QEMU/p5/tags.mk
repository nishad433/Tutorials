ARCH?=arm64

ifeq (${DEBUG},1)
MAKE_FLAGS:=-w
else
MAKE_FLAGS:=--no-print-directory
endif

ifeq (${ARCH},arm64)
TARGET:=kernel8
MACHINE:=raspi3
TOOLCHAIN:=gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf
CROSS_COMPILE:=aarch64-none-elf-
endif
ifeq (${ARCH},arm32)
TARGET:=kernel
MACHINE:=vexpress-a9
TOOLCHAIN:=arm-gnu-toolchain-12.2.mpacbti-bet1-x86_64-arm-none-eabi
CROSS_COMPILE:=arm-none-eabi-
endif

ifeq (${CONSOLE},UART1)
SERIAL_CONSOLE:=-serial null -serial stdio
else
CONSOLE:=UART0
SERIAL_CONSOLE:=-serial stdio -serial null
endif

ifeq (${MACHINE},raspi3)
DEFINES:=-DRASPI3
QEMU_CMD:=qemu-system-aarch64 -M ${MACHINE} -no-reboot ${SERIAL_CONSOLE} -kernel ${TARGET}.bin 
QEMU_GDB_CMD:=qemu-system-aarch64 -M ${MACHINE} -m 1000M -no-reboot ${SERIAL_CONSOLE} -kernel ${TARGET}.bin -S -gdb tcp::3333
endif

ifeq (${MACHINE},vexpress-a9)
DEFINES:=-DVEXP_A9
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

ifeq (${ARCH},arm64)
ASFLAGS:= -O0 -ggdb3 -march=armv8-a
endif

ifeq (${ARCH},arm32)
ASFLAGS:= -O0 -ggdb3 -march=armv7-a
endif

CFLAGS= -O0 -ggdb3 -Wall -MMD ${DEFINES} ${INCLUDES} -nostdlib -nostartfiles

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
