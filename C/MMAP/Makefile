EXE	:= 01-allocating-memory 02-reading-file 03-ipc
CFLAGS	:= -Wall -ggdb


%.o:	%.c
	$(CC) $(CFLAGS) $< -o $@

all:	$(EXE)

clean:
	rm -rf *.o $(EXE)
