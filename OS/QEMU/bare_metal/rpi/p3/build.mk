ARCH_DIR=arch/$(CONFIG_ARCH)

dirs = $(ARCH_DIR) drivers init kernel utils

# Build kernel and dependencies
.PHONY: build
build: copy_sym_section genarate_sym_section
	@cp $(DEFCONFIG_FILE) $(KCONFIG_FILE)
	@for dir in $(dirs); do \
		#echo "Entering $$dir"; \
		$(MAKE) -C $$dir || exit 1; \
	done

$(ARCH_DIR)/symbol_area.o: $(ARCH_DIR)/symbol_area.s
	@printf "%-10s %-15s %s\n" "" "[CCS]" "$(subst $(ROOT_DIR)/,,$(PWD)/)$<"
	@$(CC) $(ASFLAGS) -c $< -o $@

genarate_sym_section : $(ARCH_DIR)/symbol_area.o

.PHONY: copy_sym_section
copy_sym_section :
	@cp $(ARCH_DIR)/sample_symbol_area.txt $(ARCH_DIR)/symbol_area.s

# Clean up build artifacts
.PHONY: clean
clean:
	@for dir in $(dirs); do \
		#echo "Entering $$dir"; \
		$(MAKE) -C $$dir clean  || exit 1; \
	done
	@rm -f $(KCONFIG_FILE) $(ARCH_DIR)/symbol_area.s $(ROOT_DIR)/.nm*
	@rm -f $(KERNEL).elf $(KERNEL).bin $(KERNEL).*dump cscope.*
