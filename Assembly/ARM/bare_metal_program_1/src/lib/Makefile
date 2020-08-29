
CFLAGS+=-fpic


LIB_OBJS:=ncc.o
LIB_TARGET:=libncc.so

all:build copy

build:	$(LIB_OBJS)
	$(CROSS_CC) -shared $^ $(LDFLAGS) -o $(LIB_TARGET) 

copy:
	cp $(LIB_TARGET) ../

%.o:	%.c
	$(CROSS_CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o $(LIB_TARGET) ../$(LIB_TARGET) 
