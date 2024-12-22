SUBDIRS:=
ifeq (${MACHINE},raspi3)
SUBDIRS+=gpio
SUBDIRS+=mbox
SUBDIRS+=rng
endif
SUBDIRS+=uart

build :
	@for d in ${SUBDIRS}; do [ -d $$d ] && ${MAKE} ${MAKE_FLAGS} -C $$d; done
clean:
	@for d in ${SUBDIRS}; do [ -d $$d ] && ${MAKE} ${MAKE_FLAGS} -C $$d clean; done
