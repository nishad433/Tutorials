CC := gcc
CFLAGS := -Wall

# flags to build shared library
LIB_EXTRACFLAGS := -fPIC
LIB_LDFLAGS := -shared

# library name
LIBNAME = ringbuf
TARGET_LIB = lib$(LIBNAME).so


TARGET_EXE := test
EXE_SRCS := main.c
EXE_OBJS := $(EXE_SRCS:.c=.o)


all: $(TARGET_LIB) $(TARGET_EXE)




# Source files for the library libringbuf
LIB_SRCS := ringbuf.c

# Object files
LIB_OBJS = $(LIB_SRCS:.c=.o)

# rule to build shared library
$(TARGET_LIB): $(LIB_OBJS)
	$(CC) $(LIB_LDFLAGS) -o $@ $(LIB_OBJS)

$(TARGET_EXE): $(EXE_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(EXE_OBJS) -L. -l$(LIBNAME) -Wl,-rpath,.

%.o: %.c
ifeq ($(filter $<,$(LIB_SRCS)), $<)
	$(CC) $(CFLAGS) $(LIB_EXTRACFLAGS) -c $< -o $@
else
	$(CC) $(CFLAGS) -c $< -o $@
endif

clean:
	rm -rf *.o
	rm -rf $(TARGET_LIB) $(TARGET_EXE)
