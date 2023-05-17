#export PATH=$PATH:~/mistsys/jaime/apfw/output/aphx/host/bin/

ARCH?=x86
KDIR:=/lib/modules/$(shell uname -r)/build
ARGS:=
PWD:=$(shell pwd)


ifeq ($(ARCH),x86)
ARCH:=x86
KDIR:=/lib/modules/$(shell uname -r)/build
else
ifeq ($(ARCH),arm64)
ARCH:=arm64
KDIR:=/home/nishad/mistsys/jaime/apfw/output/aphx/build/linux-4.4.113/
COMPILER:=aarch64-buildroot-linux-uclibc-
COMPILE_ARGS:=ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER)
endif
endif

obj-m := ncc.o

ncc-objs := main.o fops.o vm_ops.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) $(COMPILE_ARGS) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) $(COMPILE_ARGS) clean
