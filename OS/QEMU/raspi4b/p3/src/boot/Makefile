s_files := ${wildcard *.S}
objs := ${s_files:%.S=%.o}
deps := ${s_files:%.S=%.d}

build: ${objs}

%.o : %.S
	@echo "Compiling $(subst $(TOP)/,,$(CURDIR)/)$<"
	@echo "$(subst $(TOP)/,,$(CURDIR)/)$@" >> ${OBJLISTFILE}
	@$(CC) ${ASFLAGS} -c $< -o $@

.PHONY : clean
clean :
	rm -rf {$(subst $(TOP)/,,$(CURDIR)/)} ${objs} ${deps}

-include ${deps}
