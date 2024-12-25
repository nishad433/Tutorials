# Include the configuration file (Kconfig)
-include $(KCONFIG_FILE)

# Set the current working directory
PWD := $(shell pwd)

# Define the directories for which make will be invoked (subdirectories with 'obj-y')
dirs = $(filter %/,$(obj-y))

# The 'all' target is invoked by the top-level Makefile
.PHONY: all
all: call_submake

# Rule to handle object files (.o) from C source files (.c)
%.o: %.c
	@printf "%-10s %-15s %s\n" "" "[CC]" "$(subst $(ROOT_DIR)/,,$(PWD)/)$<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Rule to handle object files (.o) from assembly source files (.S)
%.o: %.S
	@printf "%-10s %-15s %s\n" "" "[CC]" "$(subst $(ROOT_DIR)/,,$(PWD)/)$<"
	@$(CC) $(ASFLAGS) -c $< -o $@

# Rule to handle subdirectories, invoking 'make' in each subdirectory
.PHONY: call_submake
call_submake:
	@for dir in $(dirs); do \
		$(MAKE) -C $$dir $(MAKECMDGOALS) || exit 1; \
	done

.PHONY: clean
clean: call_submake
	@printf "%-10s %-15s %s\n" "" "[CLEAN]" "$(shell realpath --relative-to=$(ROOT_DIR) $(CURDIR))"
	@rm -rf *.o *.d
