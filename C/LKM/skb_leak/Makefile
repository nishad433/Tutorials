HOST_BIN_DIR:=/home/nishad/mistsys/mance/apfw/output/aphx/host/bin
#KDIR=/lib/modules/$(shell uname -r)/build
KDIR:=/home/nishad/mistsys/mance/apfw/output/aphx/build/linux-4.19.170
PWD:=$(shell pwd)
ARCH:=arm64
COMPILER:=$(HOST_BIN_DIR)/aarch64-buildroot-linux-uclibc-
EXTRA_CFLAGS=-I$(srctree)/mm/

MYDATE := $(shell date +"%Y-%m-%d")
MYTIME := $(shell date +"%H:%M:%S")

CFLAGS_mist_skb_debug.o += -DMYDATE=\"$(MYDATE)\" -DMYTIME=\"$(MYTIME)\"

obj-m := ncc.o

ncc-objs := mist_skb_debug.o



all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) modules
	#$(MAKE) -C $(KDIR) M=$(PWD)  modules


clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) clean
	#$(MAKE) -C $(KDIR) M=$(PWD) clean
