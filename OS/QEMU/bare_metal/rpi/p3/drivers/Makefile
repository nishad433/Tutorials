# Include the root Makefile containing build rules
include $(ROOT_DIR)/buildrules.mk

# Add subdirectories to the 'obj-y' list based on configuration options
obj-$(CONFIG_GPIO)              += gpio/
obj-$(CONFIG_AUX_PERIPH)        += aux_periph/
obj-$(CONFIG_UART)              += uart/
obj-$(CONFIG_GENERIC_TIMER)              += generic_timer/

# Define the 'all' target, which depends on the directories in 'obj-y'
all: $(obj-y)

