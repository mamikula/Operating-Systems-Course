CC = gcc
CFLAGS = -Wall -g

all: run

compile:
	@$(CC) $(FLAGS) main.c -o main -lpthread

run: compile
	./main $(ARGS)

test: compile
	./main 1 numbers dragon.ascii.pgm negative
	./main 1 block dragon.ascii.pgm negative
	./main 2 numbers dragon.ascii.pgm negative
	./main 2 block dragon.ascii.pgm negative
	./main 4 numbers dragon.ascii.pgm negative
	./main 4 block dragon.ascii.pgm negative
	./main 8 numbers dragon.ascii.pgm negative
	./main 8 block dragon.ascii.pgm negative
	./main 16 numbers dragon.ascii.pgm negative
	./main 16 block dragon.ascii.pgm negative

clean:
	@rm -f main Times.txt