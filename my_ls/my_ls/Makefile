CC = gcc
CFLAGS = -I.
DEPS = functions.h
OBJ = main.o func1.o func2.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

my_ls: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o functions