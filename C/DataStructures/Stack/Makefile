export CC:=gcc
export LD:=ld

BINARY:=ncc
PWD:=$(shell pwd)
LIB_DIR:=$(PWD)/lib_ll
LIB_INC:=$(LIB_DIR)/inc/
CFLAGS=-Wall
LDFLAGS=-L$(LIB_DIR) -Wl,-rpath,$(LIB_DIR)

all : main.o
	make -C $(LIB_DIR) libstack
	$(CC) $(LDFLAGS) main.o -o $(BINARY) -lstack 

%.o : %.c
	$(CC) $(CFLAGS) -I$(LIB_INC) -c $^ -o $@

clean :
	make -C $(LIB_DIR) clean
	rm -rf *.o $(BINARY) $(LIB_OBJS) libstack.so
