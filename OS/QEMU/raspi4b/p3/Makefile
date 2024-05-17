TOP:=$(shell pwd)

include ${TOP}/tags.mk

help :
	@echo "make all         : builds the image"
	@echo "make clean       : clean the repo"
	@echo "make run-gdb     : run the kernel in qemu with debugging enabled"
	@echo "make run         : run the kernel in qemu"
	@echo "make objdump     : build objdumps"

all : build objs
	${LD} ${objslist} -T ${LDSCRIPT} ${LDFLAGS} -o ${TARGET}.elf
	${OBJCOPY} -O binary ${TARGET}.elf ${TARGET}.bin

# finds all .o in src/*/ and src/*/*/
objs :
	${eval objslist:= $(shell cat ${OBJLISTFILE} | xargs)}

build :
	rm -rf ${OBJLISTFILE}
	@${MAKE} ${MAKE_FLAGS} -C src build

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
	@${MAKE} ${MAKE_FLAGS} -C src clean
	rm -rf ${OBJLISTF} ${TARGET}.elf ${TARGET}.bin ${TARGET}.*.dump cscope.*
