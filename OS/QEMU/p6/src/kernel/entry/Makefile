C_FILES := ${wildcard *.c}
OBJS := ${C_FILES:%.c=%.o}
DEPS := ${C_FILES:%.c=%.d}

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

build: ${OBJS}

.PHONY: clean
clean:
	rm -rf ${OBJS} ${DEPS}

-include $(DEPS)
