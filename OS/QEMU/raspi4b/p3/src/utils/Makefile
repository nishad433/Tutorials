c_files := ${wildcard *.c}
objs := ${c_files:%.c=%.o}
deps := ${c_files:%.c=%.d}

.PHONY : build announce_start announce_end clean

build: announce_start ${objs} announce_end

announce_start :
	@echo ">>> Building Utils"

announce_end :
	@echo ">>> Building Utils [done]\n"

%.o : %.c
	@echo "\tCompiling $(subst $(TOP)/,,$(CURDIR)/)$<"
	@echo "$(subst $(TOP)/,,$(CURDIR)/)$@" >> ${OBJLISTFILE}
	@$(CC) ${CFLAGS} -c $< -o $@

clean :
	rm -rf {$(subst $(TOP)/,,$(CURDIR)/)} ${objs}
	@rm -rf {$(subst $(TOP)/,,$(CURDIR)/)} ${deps}

-include ${deps}
