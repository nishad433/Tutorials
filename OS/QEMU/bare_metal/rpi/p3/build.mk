# Define architecture-specific directory
ARCH_DIR := arch/$(CONFIG_ARCH)

# List of directories to build
DIRS = $(ARCH_DIR) drivers init kernel utils

# Build the kernel and its dependencies
.PHONY: build
build: copy_sym_section generate_sym_section
	@cp $(DEFCONFIG_FILE) $(KCONFIG_FILE)
	@for dir in $(DIRS); do \
		# Uncomment the next line to see which directory is being processed \
		# echo "Entering $$dir"; \
		$(MAKE) -C $$dir || exit 1; \
	done

# Rule to build symbol_area.o from symbol_area.s
$(ARCH_DIR)/symbol_area.o: $(ARCH_DIR)/symbol_area.s
	@printf "%-10s %-15s %s\n" "" "[CC]" "$(subst $(ROOT_DIR)/,,$(PWD)/)$<"
	@$(CC) $(ASFLAGS) -c $< -o $@

# Generate the symbol section
generate_sym_section: $(ARCH_DIR)/symbol_area.o

# Copy the sample symbol area file
.PHONY: copy_sym_section
copy_sym_section:
	@cp $(ARCH_DIR)/sample_symbol_area.txt $(ARCH_DIR)/symbol_area.s

# Clean up build artifacts
.PHONY: clean
clean:
	@for dir in $(DIRS); do \
		# Uncomment the next line to see which directory is being cleaned \
		# echo "Entering $$dir"; \
		$(MAKE) -C $$dir clean || exit 1; \
	done
	@rm -f $(KCONFIG_FILE) $(ARCH_DIR)/symbol_area.s $(ROOT_DIR)/.nm*
	@rm -f $(KERNEL).elf $(KERNEL).bin $(KERNEL).*dump cscope.*
