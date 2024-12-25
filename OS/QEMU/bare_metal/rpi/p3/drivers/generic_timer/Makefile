# Include the root Makefile containing build rules
include $(ROOT_DIR)/buildrules.mk

obj-$(CONFIG_UART)              += generic_timer.o

# Define the 'all' target, which depends on the directories in 'obj-y'
all: $(obj-y)

