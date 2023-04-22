clean:
	rm -r *.o *.so zad1

lib:
	gcc -fPIC -c bibl1.c
	gcc -shared bibl1.o -o bibl1.so
	gcc zad1.c -o zad1 -ldl
	./zad1