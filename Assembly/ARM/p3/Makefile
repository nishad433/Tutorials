TARGET=p3_binary

CROSS_COMPILE?=
AS=$(CROSS_COMPILE)as
LD=$(CROSS_COMPILE)ld
CC=$(CROSS_COMPILE)gcc

%.o:	%.S ncc.h
	$(AS) -o $@ $<

%.o:	%.c ncc.h
	$(CC) -c -o $@ $<


all:	main.o asm.S
	$(CC) -o $(TARGET) $^

clean:
	rm -rf *.o $(TARGET)
