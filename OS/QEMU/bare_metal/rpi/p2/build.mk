ARCH_DIR=arch/$(CONFIG_ARCH)

dirs = $(ARCH_DIR) drivers init kernel utils

# Build kernel and dependencies
.PHONY: build
build:
	@cp $(DEFCONFIG_FILE) $(KCONFIG_FILE)
	@for dir in $(dirs); do \
		#echo "Entering $$dir"; \
		$(MAKE) -C $$dir || exit 1; \
	done

# Clean up build artifacts
.PHONY: clean
clean:
	@for dir in $(dirs); do \
		#echo "Entering $$dir"; \
		$(MAKE) -C $$dir clean  || exit 1; \
	done
	@rm -f $(KCONFIG_FILE)
	@rm -f $(KERNEL).elf $(KERNEL).bin $(KERNEL).*dump cscope.*
