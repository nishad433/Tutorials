# flags to build shared library
LIB_EXTRACFLAGS := -fPIC
LIB_LDFLAGS := -shared

TARGET_LIB:=libringbuf.so

# Source files for the library libringbuf
LIB_SRCS := ringbuf.c

# Object files
LIB_OBJS = $(LIB_SRCS:.c=.o)

# rule to build shared library
$(TARGET_LIB): $(LIB_OBJS)
	$(CC) $(LIB_LDFLAGS) -o $@ $(LIB_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(LIB_EXTRACFLAGS) -c $< -o $@

clean:
	rm -rf *.o
	rm -rf $(TARGET_LIB)
