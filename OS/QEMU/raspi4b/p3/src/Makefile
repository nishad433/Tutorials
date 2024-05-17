SUBDIRS:=boot drivers kernel utils


build : ${SUBDIRS}

${SUBDIRS} :
	@${MAKE} -C $@ build

clean :
	@for d in ${SUBDIRS}; do [ -d $$d ] && ${MAKE} -C $$d clean; done

.PHONY: build clean ${SUBDIRS}
