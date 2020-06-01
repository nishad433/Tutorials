CFLAGS=-g
all:lib
	$(CC) $(CFLAGS) main.c linked_list.o -o test

lib:
	$(CC) $(CFLAGS) -c linked_list.c

clean:
	rm -rf *.o
	rm -rf test
