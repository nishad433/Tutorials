# Debug settings
ifeq ($(DEBUG), 1)
    MAKE_FLAGS := -w
else
    MAKE_FLAGS := --no-print-directory
endif

# Toolchain configuration
TOOLCHAIN_DIR     := /home/nishad/x-tools/aarch64-unknown-linux-gnu
TOOLCHAIN_BIN     := $(TOOLCHAIN_DIR)/bin
TOOLCHAIN_INCLUDE := $(TOOLCHAIN_DIR)/aarch64-unknown-linux-gnu/sysroot/usr/include

# Add toolchain binaries to PATH
PATH := $(PATH):$(TOOLCHAIN_BIN)

# Cross-compiler tools
CROSS_COMPILE := aarch64-unknown-linux-gnu-
AS            := $(CROSS_COMPILE)as
CC            := $(CROSS_COMPILE)gcc
LD            := $(CROSS_COMPILE)ld
OBJCOPY       := $(CROSS_COMPILE)objcopy
OBJDUMP       := $(CROSS_COMPILE)objdump

# Compiler and assembler flags
INCLUDES := -I$(ROOT_DIR)/include -I$(TOOLCHAIN_INCLUDE)
DEFINES  :=
ASFLAGS  := -O0 -ggdb3 -march=armv8-a -mgeneral-regs-only $(DEFINES) $(INCLUDES)
CFLAGS   := -O0 -ggdb3 -Wall -MMD -mgeneral-regs-only -nostdlib -nostartfiles $(DEFINES) $(INCLUDES)

# Linker settings
LDFLAGS       :=
LINKER_SCRIPT := link64.ld

# Target settings
KERNEL := kernel8

# Build configs
DEFCONFIG_FILE := $(ROOT_DIR)/arch/arm64/configs/raspi4b_defconfig
KCONFIG_FILE := $(ROOT_DIR)/.config
include $(DEFCONFIG_FILE)

# MAKE settings
MAKE :=make $(MAKE_FLAGS)

# QEMU configuration
QEMU		  := qemu-system-aarch64
QEMU_DIR          := /home/nishad/Documents/qemu/build
QEMU_SYSTEM       := $(QEMU_DIR)/$(QEMU)
SERIAL_CONSOLE    := -serial null -serial stdio
QEMU_CMD          := $(QEMU_SYSTEM) -M raspi4b -smp 4 -m 2048M -no-reboot $(SERIAL_CONSOLE) -kernel $(KERNEL).bin
QEMU_GDB_CMD      := $(QEMU_CMD) -S -gdb tcp::3333

# Export variables for sub-make processes
export PATH
export ROOT_DIR PLATFORM KCONFIG_FILE DEFCONFIG_FILE
export AS CC LD OBJCOPY OBJDUMP
export ASFLAGS CFLAGS LDFLAGS MAKE

