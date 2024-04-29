ifeq (${DEBUG},1)
MAKE_FLAGS:=-w
else
MAKE_FLAGS:=--no-print-directory
endif

# TOOLCHAIN related
TOOLCHAIN_ROOT:=/home/nishad/Documents/toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf
TOOLCHAIN_PATH:=${TOOLCHAIN_ROOT}
TOOLCHAIN_BIN:=${TOOLCHAIN_PATH}/bin
PATH=$(shell printenv PATH):${TOOLCHAIN_BIN}
CROSS_COMPILE:=aarch64-none-elf-
AS:=${CROSS_COMPILE}as
CC:=${CROSS_COMPILE}gcc
LD:=${CROSS_COMPILE}ld
OBJCOPY:=${CROSS_COMPILE}objcopy
OBJDUMP:=${CROSS_COMPILE}objdump

DEFINES:=
INCLUDES:=-I${TOP}/include/
ASFLAGS:= -O0 -ggdb3 -g3 -march=armv8-a ${DEFINES} ${INCLUDES}
CFLAGS= -O0 -ggdb3 -g3 -Wall -MMD ${DEFINES} ${INCLUDES} -nostdlib -nostartfiles
LDFLAGS:=
LDSCRIPT:=link64.ld

# TARGET machine related
TARGET:=kernel8
MACHINE:=raspi4b

QEMU_PATH:=/home/nishad/Documents/qemu/build
QEMU_SYSTEM:=${QEMU_PATH}/qemu-system-aarch64
QEMU_CMD:=${QEMU_SYSTEM} -M ${MACHINE} -smp 4 -m 2048M  -no-reboot -nographic -kernel ${TARGET}.bin -d in_asm
QEMU_GDB_CMD:=${QEMU_SYSTEM} -M ${MACHINE} -smp 4 -m 2048M -no-reboot ${SERIAL_CONSOLE} -kernel ${TARGET}.bin -S -gdb tcp::3333



export PATH
export AS CC LD OBJCOPY OBJDUMP
export ASFLAGS CFLAGS LDFLAGS MAKE_FLAGS
