SUBDIRS   := boot c

all:
	for dir in $(SUBDIRS); do \
                make -C $$dir build; \
        done

clean:
	for dir in $(SUBDIRS); do \
                make -C $$dir clean; \
        done



