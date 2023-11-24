PWD:=$(shell pwd)
LIB_DIR:=$(PWD)
LIB_INC:=$(LIB_DIR)/inc/
LIB_SRC:=$(wildcard $(LIB_DIR)/src/*.c)
LIB_OBJS:=$(patsubst %.c,%.o,$(LIB_SRC))
CFLAGS=-Wall -fPIC

%.o : %.c
	$(CC) $(CFLAGS) -I$(LIB_INC) -c $^ -o $@

libstack : $(LIB_OBJS)
	$(CC) $(LIB_OBJS) -shared -o libstack.so

clean :
	rm -rf src/*.o libstack.so
