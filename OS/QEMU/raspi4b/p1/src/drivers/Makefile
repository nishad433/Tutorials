SUBDIRS:=aux_periph gpio uart


build : ${SUBDIRS}

${SUBDIRS} :
	${MAKE} -C $@ build

clean :
	@for d in ${SUBDIRS}; do [ -d $$d ] && ${MAKE} -C $$d clean; done

.PHONY: build clean ${SUBDIRS}
