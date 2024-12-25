# Include the root Makefile containing build rules
include $(ROOT_DIR)/buildrules.mk

obj-y				+= cache.o
obj-y				 += generic_timer.o
obj-y				 += tlb.o
obj-y				 += symbol_area.o
obj-$(CONFIG_ARM64)              += boot.o
obj-$(CONFIG_ARM64)              += irq64.o
obj-$(CONFIG_ARM64)              += vectors64.o


# Define the 'all' target, which depends on the directories in 'obj-y'
all: $(obj-y)

