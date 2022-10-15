
OBJS := main.o \
	uart.o \
	printk.o

ifeq (${ARCH},arm32)
#C_OBJS+=${BOOT_SRC}/boot32.o
else
ifeq (${ARCH},arm64)
OBJS+=gpio.o
endif
endif

CFLAGS+=-I${C_SRC}/include/

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

build: ${OBJS}

.PHONY: clean
clean :
	rm -rf ${OBJS} *.o	
