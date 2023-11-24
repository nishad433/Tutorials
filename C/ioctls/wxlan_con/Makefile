#export PATH=$PATH:/home/nishad/mistsys/mance_28548/apfw/output/aphx/host/bin/
#export PATH=$PATH:/home/nishad/mistsys/lollys/apfw/output/aphx/host/bin/

CROSS :=aarch64-buildroot-linux-uclibc-
CC :=$(CROSS)gcc
LD :=$(CROSS)ld


BIN := ncc
APP := ncc_user

#INCLUDES :=-I/home/nishad/mistsys/mance_28548/mist-wxlan/include
#INCLUDES +=-I/home/nishad/mistsys/mance_28548/apfw/files/linux/4.4.302/files/include/ 
INCLUDES :=-I/home/nishad/mistsys/master/mist-wxlan/include
INCLUDES +=-I/home/nishad/mistsys/master/apfw/files/linux/4.19.170/files/include/ 

DEFINES :=-DTARGET_aphx

%.o : %.c
	$(CC) $(DEFINES) $(INCLUDES) -c $^

all : wxlan_bridge.o
	$(CC) $^ -o $(BIN)

app : ncc_pm_test.o
	$(CC) $^ -o $(APP)

clean :
	rm -rf *.o
	rm -rf $(BIN) $(APP)
