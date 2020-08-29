#KDIR=/lib/modules/$(shell uname -r)/build
#KDIR=/home/nishad/mistsys/apfw/output/wap/build/linux-4.4.113/
KDIR=/home/nishad/mistsys/apfw/output/aph/build/linux-3.10.105/
PWD=$(shell pwd)


ARCH:=arm
COMPILER:=arm-buildroot-linux-uclibcgnueabi-


obj-m := ncc.o

ncc-objs := main.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) modules


clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER)  clean
