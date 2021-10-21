CC = gcc
CFLAGS = -I.
DEPS = functions.h
OBJ = main.o get_options.o other_functions.o options_create.o options_execute.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

my_tar: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o functions