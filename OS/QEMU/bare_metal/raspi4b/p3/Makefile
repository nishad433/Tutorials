TOP:=$(shell pwd)

include ${TOP}/tags.mk

help :
	@echo "make all         : builds the image"
	@echo "make clean       : clean the repo"
	@echo "make run-gdb     : run the kernel in qemu with debugging enabled"
	@echo "make run         : run the kernel in qemu"
	@echo "make objdump     : build objdumps"

all : cleanup build
	${eval objslist:= $(shell cat ${OBJLISTFILE} | xargs)}
	@${LD} ${objslist} src/boot/symbol_area.o -T ${LDSCRIPT} ${LDFLAGS} -o ${TARGET}.elf
	@# get list of all symbols, skip the ones starting with '$'
	@${NM} -n ${TARGET}.elf | sort | grep -v '\$$' | awk '{if($$2=="t"|| $$2=="T")print $$0}' > .nmfirstpass
	@# get list of all symbols, skip the ones starting with '$', pass it script to generate symbol_area.s
	@${NM} -n ${TARGET}.elf | sort | grep -v '\$$' | awk '{if($$2=="t"|| $$2=="T")print $$0}' | scripts/generate_syms.py
	@# compile symbol_area.s
	@${MAKE} ${MAKE_FLAGS} -C src/boot build_syms
	@# build kernel by link all object files (including symbol_area.o)
	@echo "\n>>> Generating ${TARGET}.elf & ${TARGET}.bin"
	@${LD} ${objslist} src/boot/symbol_area.o -T ${LDSCRIPT} ${LDFLAGS} -o ${TARGET}.elf
	@# just to compare and verify that adding symbol_area.o doesnt add any new symbols.
	@${NM} -n ${TARGET}.elf | sort | grep -v '\$$' | awk '{if($$2=="t"|| $$2=="T")print $$0}' > .nmsecondpass
	@${OBJCOPY} -O binary ${TARGET}.elf ${TARGET}.bin

cleanup :
	@rm -rf .nm* src/boot/symbol_area.*

build :
	@${MAKE} ${MAKE_FLAGS} -C src/boot copy
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
	@${MAKE} --no-print-directory -C src clean
	rm -rf ${OBJLISTF} ${TARGET}.elf ${TARGET}.bin ${TARGET}.*.dump cscope.* .nmfirstpass .nmsecondpass
