HOST_BIN_DIR:=~/mistsys/mance/apfw/output/aphx/host/bin
#KDIR=/lib/modules/$(shell uname -r)/build
KDIR:=/home/nishad/mistsys/mance/apfw/output/aphx/build/linux-4.19.170
PWD:=$(shell pwd)
ARCH:=arm64
COMPILER:=$(HOST_BIN_DIR)/aarch64-buildroot-linux-uclibc-
#EXTRA_CFLAGS=-I$(srctree)/mm/

obj-m := ncc.o

ncc-objs := main.o



all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) modules


clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) clean
