# flags to build shared library
LIB_EXTRACFLAGS := -fPIC
LIB_LDFLAGS := -shared

TARGET_LIB_A:=libstack_a.so
TARGET_LIB_LL:=libstack_ll.so

# Source files for the library libringbuf
LIB_A_SRCS := stack_a.c
LIB_LL_SRCS := stack_ll.c

# Object files
LIB_A_OBJS = $(LIB_A_SRCS:.c=.o)
LIB_LL_OBJS = $(LIB_LL_SRCS:.c=.o)

# rule to build shared library
$(TARGET_LIB_A): $(LIB_A_OBJS)
	$(CC) $(LIB_LDFLAGS) -o $@ $(LIB_A_OBJS)

$(TARGET_LIB_LL): $(LIB_LL_OBJS)
	$(CC) $(LIB_LDFLAGS) -o $@ $(LIB_LL_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(LIB_EXTRACFLAGS) -c $< -o $@

clean:
	rm -rf *.o
	rm -rf *.so
