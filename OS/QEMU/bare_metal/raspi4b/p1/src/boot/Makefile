s_files := ${wildcard *.S}
objs := ${s_files:%.S=%.o}
deps := ${s_files:%.S=%.d}

build: ${objs}

%.o : %.S
	$(CC) ${ASFLAGS} -c $< -o $@

.PHONY : clean
clean :
	rm -rf ${objs} ${deps}

-include ${deps}
