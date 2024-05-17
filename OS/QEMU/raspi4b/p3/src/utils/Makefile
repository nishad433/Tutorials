c_files := ${wildcard *.c}
objs := ${c_files:%.c=%.o}
deps := ${c_files:%.c=%.d}

build: ${objs}

%.o : %.c
	@echo "Compiling $(subst $(TOP)/,,$(CURDIR)/)$<"
	@echo "$(subst $(TOP)/,,$(CURDIR)/)$@" >> ${OBJLISTFILE}
	@$(CC) ${CFLAGS} -c $< -o $@

.PHONY : clean
clean :
	rm -rf {$(subst $(TOP)/,,$(CURDIR)/)} ${objs} ${deps}

-include ${deps}
