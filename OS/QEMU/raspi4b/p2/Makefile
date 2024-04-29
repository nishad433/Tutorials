TOP:=$(shell pwd)

include ${TOP}/tags.mk

help :
	@echo "make all         : builds the image"
	@echo "make clean       : clean the repo"
	@echo "make run-gdb     : run the kernel in qemu with debugging enabled"
	@echo "make run         : run the kernel in qemu"
	@echo "make objdump     : build objdumps"

all : build objs
	${LD} ${objs} -T ${LDSCRIPT} ${LDFLAGS} -o ${TARGET}.elf
	${OBJCOPY} -O binary ${TARGET}.elf ${TARGET}.bin

# finds all .o in src/*/ and src/*/*/
objs :
	${eval objs:= $(shell realpath --relative-to ${TOP} `find ${TOP} -name *.o`)}

build :
	${MAKE} ${MAKE_FLAGS} -C src

run-gdb :
	${QEMU_GDB_CMD}

run :
	${QEMU_CMD}

objdump :
	${OBJDUMP} -d ${TARGET}.elf > ${TARGET}.text.dump
	${OBJDUMP} -D ${TARGET}.elf > ${TARGET}.all.dump
	${OBJDUMP} -t ${TARGET}.elf > ${TARGET}.syms.dump

.PHONY: clean
clean :
	${MAKE} ${MAKE_FLAGS} -C src clean
	rm -rf ${TARGET}.elf ${TARGET}.bin ${TARGET}.*.dump
