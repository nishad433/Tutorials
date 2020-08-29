
TARGET=test

export CROSS_CC_DIR=/home/nishad/mistsys/apfw/output/aplx/host/bin/
export CC=gcc
export ARCH=arm

export CFLAGS:=-g
LDFLAGS:=-L. -Wl,-rpath=.

export CROSS_CC=$(CROSS_CC_DIR)$(ARCH)-buildroot-linux-uclibcgnueabi-$(CC)
#export CROSS_CC:=$(CC)


OBJS=main.o

all:	libs build

build:	$(OBJS)
	$(CROSS_CC) $(OBJS) $(LDFLAGS) -lncc -o $(TARGET) 

libs:
	$(MAKE) -C ./lib/

%.o:	%.c
	$(CROSS_CC) $(CFLAGS) -c $<



clean:
	rm -rf *.o $(TARGET)
	$(MAKE) -C lib/ clean
