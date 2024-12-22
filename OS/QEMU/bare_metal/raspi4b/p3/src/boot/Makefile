s_files := ${wildcard *.S}
objs := ${s_files:%.S=%.o}
deps := ${s_files:%.S=%.d}

.PHONY : build announce_start announce_end build_syms clean

build: announce_start ${objs} build_syms announce_end

announce_start :
	@echo ">>> Building Boot"

announce_end :
	@echo ">>> Building Boot [done]\n"

%.o : %.S
	@echo "\tCompiling $(subst $(TOP)/,,$(CURDIR)/)$<"
	@echo "$(subst $(TOP)/,,$(CURDIR)/)$@" >> ${OBJLISTFILE}
	@$(CC) ${ASFLAGS} -c $< -o $@

build_syms: symbol_area.s
	#@echo "\tCompiling $(subst $(TOP)/,,$(CURDIR)/)$<"
	@$(CC) ${ASFLAGS} -c $< -o symbol_area.o

copy :
	@cp sample_symbol_area.txt symbol_area.s

clean :
	rm -rf {$(subst $(TOP)/,,$(CURDIR)/)} ${objs} symbol_area.o
	@rm -rf {$(subst $(TOP)/,,$(CURDIR)/)} symbol_area.* ${deps}

-include ${deps}
