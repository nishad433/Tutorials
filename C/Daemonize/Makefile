#\
 =       Assign the value to the variable.  Any previous value is overridden. 	\
										\
 +=      Append the value to the current value of the variable.			\
										\
 ?=      Assign the value to the variable if it is not already defined.		\
										\
 :=      Assign with expansion, i.e. expand the value before assigning it	\
         to the variable.  Normally, expansion is not done until the vari-	\
         able is referenced.  NOTE: References to undefined variables are	\
         not expanded.  This can cause problems when variable modifiers		\
         are used.								\
										\
 !=      Expand the value and pass it to the shell for execution and		\
         assign the result to the variable.  Any newlines in the result		\
         are replaced with spaces.


TARGET = nishad-daemon
CC ?= gcc
CFLAGS ?=
LDFLAGS ?=


print:
	@echo "TARGET=$(TARGET) CC=$(CC) CFLAGS=$(CFLAGS) LDFLAGS=$(LDFLAGS)"


%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@


all: 	print build

build: 	main.o
	$(CC) $(LDFLAGS) $^ -o $(TARGET)

clean:
	rm -rf *.o $(TARGET)
