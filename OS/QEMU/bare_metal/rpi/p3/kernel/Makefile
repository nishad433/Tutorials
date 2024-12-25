# Include the root Makefile containing build rules
include $(ROOT_DIR)/buildrules.mk

obj-y              += utils.o
obj-y              += gic.o
obj-y              += debug.o
obj-y              += mm.o
obj-y              += cache.o
obj-y              += entry.o
obj-y              += mmu_table.o
obj-y              += irq.o

# Define the 'all' target, which depends on the directories in 'obj-y'
all: $(obj-y)

