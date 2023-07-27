#KDIR=/lib/modules/$(shell uname -r)/build
KDIR:=/home/nishad/mistsys/jaime/apfw/output/aphx/build/linux-4.4.113
PWD:=$(shell pwd)
ARCH:=arm64
COMPILER:=aarch64-buildroot-linux-uclibc-
EXTRA_CFLAGS=-I$(srctree)/mm/

obj-m := ncc.o

ncc-objs := main.o



all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) modules
	#$(MAKE) -C $(KDIR) M=$(PWD)  modules


clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) clean
	#$(MAKE) -C $(KDIR) M=$(PWD) clean
