
ARCH?=x86
KDIR:=/lib/modules/$(shell uname -r)/build
ARGS:=
PWD:=$(shell pwd)


ifeq ($(ARCH),x86)
ARCH:=x86
KDIR:=/lib/modules/$(shell uname -r)/build
else
ifeq ($(ARCH),arm)
ARCH:=arm
KDIR:=/home/nishad/mistsys/apfw/output/aph/build/linux-3.10.105/
COMPILER:=arm-buildroot-linux-uclibcgnueabi-
COMPILE_ARGS:=ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER)
endif
endif

obj-m := ncc.o

ncc-objs := main.o fops.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) $(COMPILE_ARGS) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) $(COMPILE_ARGS) clean
