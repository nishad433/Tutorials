M?=vexpress-a9

TARGET:=kernel

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
CROSS_COMPILE:=arm-none-eabi-
LDSCRIPT:=link32.ld
ASFLAGS+= -march=armv7-a
else
ifeq (${ARCH},arm64)
CROSS_COMPILE:=aarch64-linux-gnu-
LDSCRIPT:=link64.ld
ASFLAGS+= -march=armv8-a
endif
endif

AS:=${CROSS_COMPILE}as
CC:=${CROSS_COMPILE}gcc
LD:=${CROSS_COMPILE}ld
OBJCOPY:=${CROSS_COMPILE}objcopy

ASFLAGS:= -g3
CFLAGS:= -g3 -Wall -ffreestanding ${DEFINES}
LDFLAGS:= -nostdlib -nostartfiles
#LDFLAGS = -nostdlib -nostartfiles -lgcc -L./lib/


C_SRC=${TOP}/src/c/
BOOT_SRC=${TOP}/src/boot/

export TARGET ARCH CROSS_COMPILE LDSCRIPT ASFLAGS QEMU_GDB_CMD QEMU_CMD OBJS_LIST AS CC LD OBJCOPY CFLAGS LDFLAGS C_SRC BOOT_SRC
