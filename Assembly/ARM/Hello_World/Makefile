TARGET=hello_world

CROSS_COMPILE?=
AS=$(CROSS_COMPILE)as
LD=$(CROSS_COMPILE)ld

%.o:	%.S
	$(AS) -o $@ $<

all:	main.o
	$(LD) -o $(TARGET) $<

clean:
	rm -rf *.o $(TARGET)
