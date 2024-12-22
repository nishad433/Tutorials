ifeq (${ARCH},arm64)
ASM_FILES := ${wildcard *64.S}
endif

ifeq (${ARCH},arm32)
ASM_FILES := ${wildcard *32.S}
endif

OBJS := ${ASM_FILES:%.S=%.o}
DEPS := ${ASM_FILES:%.S=%.d}

%.o : %.S
	${CC} ${ASFLAGS} -c $< -o $@

build: ${OBJS}

.PHONY: clean
clean:
	rm -rf ${OBJS} ${DEPS}

-include $(DEPS)
