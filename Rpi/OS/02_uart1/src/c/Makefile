INCLUDES = -I$(TOP)/include/

CFLAGS += $(INCLUDES)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ 

all: gpio.o uart.o printk.o main.o 

.PHONY: clean
clean:
	rm -rf *.o
