TARGET=p2_binary

CROSS_COMPILE?=
AS=$(CROSS_COMPILE)as
LD=$(CROSS_COMPILE)ld
CC=$(CROSS_COMPILE)gcc

%.o:	%.S
	$(AS) -o $@ $<

all:	main.o
	$(CC) -o $(TARGET) $<

clean:
	rm -rf *.o $(TARGET)
