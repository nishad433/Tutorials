SUBDIRS:=${wildcard */}

build :
	@for d in ${SUBDIRS}; do [ -d $$d ] && ${MAKE} ${MAKE_FLAGS} -C $$d; done
clean:
	@for d in ${SUBDIRS}; do [ -d $$d ] && ${MAKE} ${MAKE_FLAGS} -C $$d clean; done
