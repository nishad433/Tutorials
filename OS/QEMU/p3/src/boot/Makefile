ifeq (${ARCH},arm32)
OBJS:=boot32.o
else
ifeq (${ARCH},arm64)
OBJS:=boot64.o
endif
endif

%.o : %.S
	${CC} ${ASFLAGS} -c $< -o $@

build:	${OBJS}


.PHONY: clean
clean :
	rm -rf ${OBJS} *.o
