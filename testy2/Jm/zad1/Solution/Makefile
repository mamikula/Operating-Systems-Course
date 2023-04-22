run:
	./zad1
compile:
	gcc -fPIC -c bibl1.c
	gcc -shared bibl1.o -o libbibl1.so
	gcc zad1.c -o zad1 -ldl
