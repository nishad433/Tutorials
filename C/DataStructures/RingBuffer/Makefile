PWD:=$(shell pwd)
CC := gcc

TARGET_EXE := test
EXE_SRCS := main.c
EXE_OBJS := $(EXE_SRCS:.c=.o)
LIBNAME := ringbuf
TARGET_LIB := lib$(LIBNAME).so

CFLAGS := -Wall -I$(PWD)/inc
LDFLAGS := -L$(PWD)/lib -l$(LIBNAME) -Wl,-rpath,$(PWD)/lib

all: $(TARGET_LIB) $(TARGET_EXE)

$(TARGET_LIB):
	$(MAKE) -C lib $@ CFLAGS="$(CFLAGS)"

$(TARGET_EXE): $(EXE_OBJS)
	$(CC) -o $@ $(EXE_OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C lib $@
	rm -rf *.o
	rm -rf $(TARGET_EXE)
