c_files := ${wildcard *.c}
objs := ${c_files:%.c=%.o}
deps := ${c_files:%.c=%.d}

build: ${objs}

%.o : %.c
	$(CC) ${CFLAGS} -c $< -o $@

.PHONY : clean
clean :
	rm -rf ${objs} ${deps}

-include ${deps}
