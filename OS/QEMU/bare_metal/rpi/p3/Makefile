# Root directory of the project
ROOT_DIR := $(shell pwd)

# Set the target platform (default: raspi4b)
PLATFORM ?= raspi4b

# Supported platforms
PLATFORMS := raspi3 raspi4b

# Include platform-specific makefiles
ifeq ($(PLATFORM), raspi4b)
    include $(ROOT_DIR)/raspi4b.mk
else ifeq ($(PLATFORM), raspi3)
    include $(ROOT_DIR)/raspi3.mk
else
    $(error "Unsupported PLATFORM: $(PLATFORM). Use one of raspi3 or raspi4b.")
endif

# Default tools (overridden by platform-specific makefiles)
LD ?= ld
OBJCOPY ?= objcopy
OBJDUMP ?= objdump
LINKER_SCRIPT ?= linker.ld
LINKER_FLAGS ?=

# Help target - displays usage instructions
.PHONY: help
help:
	@printf "make %-10s PLATFORM=<%s> %-10s: builds the image\n" "all" "$(PLATFORMS)" ""
	@printf "make %-10s PLATFORM=<%s> %-10s: clean the repo\n" "clean" "$(PLATFORMS)" ""
	@printf "make %-10s PLATFORM=<%s> %-10s: run the kernel in QEMU with debugging enabled\n" "run-gdb" "$(PLATFORMS)" ""
	@printf "make %-10s PLATFORM=<%s> %-10s: run the kernel in QEMU\n" "run" "$(PLATFORMS)" ""
	@printf "make %-10s PLATFORM=<%s> %-10s: generate object dumps\n" "objdump" "$(PLATFORMS)" ""

# Default build target
.PHONY: all
all: build objects
	@# Link all objects files and get the kernel elf
	@printf "%-10s %-15s %s\n" "" "[LD]" "$(KERNEL).elf"
	@$(LD) $(OBJS) -T $(LINKER_SCRIPT) $(LINKER_FLAGS) -o $(KERNEL).elf

	@# Generate symbol listings
	@printf "%-10s %-15s %s\n" "" "[NM]" "$(KERNEL).elf"
	@$(NM) -n $(KERNEL).elf | sort | grep -v '\$$' | awk '{if($$2=="t" || $$2=="T") print $$0}' > .nmfirstpass

	@# Generate symbol_area.s using the symbol table
	@printf "%-10s %-15s %s\n" "" "[GEN]" "$(KERNEL).elf [syms]"
	@$(NM) -n $(KERNEL).elf | sort | grep -v '\$$' | awk '{if($$2=="t"|| $$2=="T")print $$0}' | scripts/generate_syms.py

	@# Compile symbol_area.s
	@$(MAKE) -C ./ generate_sym_section

	@# Final linking of object files (with new symbol_area.o)
	@printf "%-10s %-15s %s\n" "" "[LD]" "$(KERNEL).elf"
	@$(LD) $(OBJS) -T $(LINKER_SCRIPT) $(LINKER_FLAGS) -o $(KERNEL).elf

	@# Ensure no new symbols are introduced by symbol_area.o
	@printf "%-10s %-15s %s\n" "" "[NM]" "$(KERNEL).elf"
	@$(NM) -n $(KERNEL).elf | sort | grep -v '\$$' | awk '{if($$2=="t"|| $$2=="T")print $$0}' > .nmsecondpass

	@printf "%-10s %-15s %s\n" "" "[OBJCOPY]" "$(KERNEL).bin"
	@$(OBJCOPY) -O binary $(KERNEL).elf $(KERNEL).bin

# Include additional build rules
include $(ROOT_DIR)/build.mk

# Locate object files
objects:
	$(eval OBJS := $(shell find $(ROOT_DIR) -name '*.o'))

# Run the kernel with GDB
.PHONY: run-gdb
run-gdb:
	$(QEMU_GDB)

# Run the kernel
.PHONY: run
run:
	$(QEMU_CMD)

# Generate object dumps
.PHONY: objdump
objdump:
	$(OBJDUMP) -d $(KERNEL).elf > $(KERNEL).text.dump
	$(OBJDUMP) -D $(KERNEL).elf > $(KERNEL).all.dump
	$(OBJDUMP) -t $(KERNEL).elf > $(KERNEL).syms.dump

