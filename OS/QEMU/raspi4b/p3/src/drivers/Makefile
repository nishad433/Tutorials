SUBDIRS:=aux_periph generic_timer gpio uart


build : announce_start ${SUBDIRS} announce_end

announce_start :
	@echo ">>> Building Drivers"

announce_end :
	@echo ">>> Building Drivers [done]\n"

${SUBDIRS} :
	@${MAKE} ${MAKE_FLAGS} -C $@ build

clean :
	@for d in ${SUBDIRS}; do [ -d $$d ] && ${MAKE} -C $$d clean; done

.PHONY: build clean ${SUBDIRS} announce_start announce_end
