M?=raspi3

TARGET:=kernel

TOOLCHAIN_ROOT:=/home/nishad/Documents//toolchain/

ifeq (${M},vexpress-a9)
ARCH=arm32
DEFINES:=-DVEXP_A9

QEMU_GDB_CMD:=qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -kernel ${TARGET}.bin -S -gdb tcp::3333
#QEMU_CMD:=qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -kernel ${TARGET}.bin -d in_asm
QEMU_CMD:=qemu-system-arm -M vexpress-a9 -m 512M -no-reboot -nographic -kernel ${TARGET}.bin 
else
ifeq (${M},raspi3)
ARCH=arm64
DEFINES:=-DRASPI3

#QEMU_CMD:=qemu-system-aarch64 -M raspi3 -no-reboot -nographic -kernel ${TARGET}.bin -d in_asm
QEMU_CMD:=qemu-system-aarch64 -M raspi3 -no-reboot -serial null -serial stdio -kernel ${TARGET}.bin 
QEMU_GDB_CMD:=qemu-system-aarch64 -M raspi3 -m 1000M -no-reboot -nographic -kernel ${TARGET}.bin -S -gdb tcp::3333
endif
endif


ifeq (${ARCH},arm32)
TOOLCHAIN_PATH:=${TOOLCHAIN_ROOT}/arm/
TOOLCHAIN_LIB:=${TOOLCHAIN_PATH}/
CROSS_COMPILE:=arm-none-eabi-
LDSCRIPT:=link32.ld
ASFLAGS+= -march=armv7-a -mcpu=cortex-a9
LDFLAGS:= -L${TOOLCHAIN_PATH}/arm-none-eabi/lib/ -lc
LDFLAGS+= -L${TOOLCHAIN_PATH}/lib/gcc/arm-none-eabi/12.2.0/ -lgcc
LDFLAGS+= -L${TOOLCHAIN_PATH}/arm-none-eabi/lib/ -lm
else
ifeq (${ARCH},arm64)
TOOLCHAIN_PATH:=${TOOLCHAIN_ROOT}/aarch64/
CROSS_COMPILE:=aarch64-none-elf-
LDSCRIPT:=link64.ld
ASFLAGS+= -march=armv8-a
LDFLAGS:= -L${TOOLCHAIN_PATH}/aarch64-none-elf/lib/ -lc
LDFLAGS+= -L${TOOLCHAIN_PATH}/lib/gcc/aarch64-none-elf/11.2.1/ -lgcc
LDFLAGS+= -L${TOOLCHAIN_PATH}/aarch64-none-elf/lib/ -lm
endif
endif

TOOLCHAIN_BIN:=${TOOLCHAIN_PATH}/bin/

AS:=${CROSS_COMPILE}as
CC:=${CROSS_COMPILE}gcc
LD:=${CROSS_COMPILE}ld
OBJCOPY:=${CROSS_COMPILE}objcopy

ASFLAGS:= -g3
CFLAGS:= -g3 -Wall ${DEFINES} -nostdlib -nostartfiles


C_SRC=${TOP}/src/c/
BOOT_SRC=${TOP}/src/boot/

export TARGET ARCH CROSS_COMPILE LDSCRIPT ASFLAGS QEMU_GDB_CMD QEMU_CMD OBJS_LIST AS CC LD OBJCOPY CFLAGS LDFLAGS C_SRC BOOT_SRC
