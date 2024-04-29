subdirs:=${wildcard */}

build :
	@for d in ${subdirs}; do [ -d $$d ] && ${MAKE} -C $$d; done
clean:
	@for d in ${subdirs}; do [ -d $$d ] && ${MAKE} -C $$d clean; done
