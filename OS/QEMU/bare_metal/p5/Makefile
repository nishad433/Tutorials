TOP:=${shell pwd}

include ${TOP}/tags.mk

all : build objs
	${LD} ${OBJS} -T ${LDSCRIPT} ${LDFLAGS} -o ${TARGET}.elf
	${OBJCOPY} -O binary ${TARGET}.elf ${TARGET}.bin

# finds all .o in src/*/ and src/*/*/
objs :
	${eval OBJS:= $(wildcard src/*/*.o src/*/*/*.o)} 

build : 
	${MAKE} ${MAKE_FLAGS} -C src

run-gdb :
	${QEMU_GDB_CMD}

run :
	${QEMU_CMD}

.PHONY: clean
clean :
	${MAKE} ${MAKE_FLAGS} -C src clean
	rm -rf ${TARGET}.elf ${TARGET}.bin
