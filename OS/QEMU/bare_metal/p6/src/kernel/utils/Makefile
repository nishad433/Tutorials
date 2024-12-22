ifeq (${ARCH},arm64)
ASM_FILES := ${wildcard *64.S}
endif
ifeq (${ARCH},arm32)
ASM_FILES := ${wildcard *32.S}
endif

C_FILES := ${wildcard *.c}

OBJS := ${ASM_FILES:%.S=%.o}
OBJS += ${C_FILES:%.c=%.o}

DEPS := ${ASM_FILES:%.S=%.d}
DEPS += ${C_FILES:%.c=%.d}

%.o : %.S
	${CC} ${ASFLAGS} -c $< -o $@

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

build: ${OBJS}

.PHONY: clean
clean:
	rm -rf ${OBJS} ${DEPS}

-include $(DEPS)
