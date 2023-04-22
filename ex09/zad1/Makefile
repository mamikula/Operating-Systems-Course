CC=gcc
CFLAGS= -Wall


all:
	make main

main: main.c
	$(CC) $(CFLAGS) main.c -o main -pthread

test: main
	./main

clean:
	rm -f main *.o *.a