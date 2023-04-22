run:
	./zad1

compile:
	gcc -fPIC -c bibl1.c
	gcc -shared bibl1.o -o bibl1.so
	gcc zad1.c -o zad1 -ldl

clean:
	rm -r *.so *.o zad1

all: compile run clean