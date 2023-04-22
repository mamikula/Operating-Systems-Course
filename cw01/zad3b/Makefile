CC = gcc
CFLAGS = -Wall -g
OPT=O

static:
	$(CC) $(CFLAGS) -c mylibrary.c -o mylibrary.o
	ar rcs libmylibrary.a mylibrary.o


shared:
	$(CC) -fPIC -c mylibrary.c
	$(CC) -shared mylibrary.o -o libmylibrary.so

test_all:
	for test in test_static test_shared test_dynamic ; do \
    		for o in 0 1 2 3 s ; do \
				printf "OPTIMALIZATION LEVEL -O$$o\n\n" >> results3b.txt; \
    			make $$test OPT=$$o; \
    		done ; \
    	done

clean:
	rm -rf results3b.txt tmpf.txt *.o *.a *.so main



test_static: static
	$(CC) -c main.c
	$(CC) main.o libmylibrary.a -o main -O${OPT}
	@echo "------STATIC TESTS------" >> results3b.txt
	make --no-print-directory small >> results3b.txt
	make --no-print-directory medium >> results3b.txt
	make --no-print-directory large >> results3b.txt

test_shared: shared
	$(CC) -c main.c
	$(CC) main.o  -L. -lmylibrary -Wl,-rpath=. -o main -O${OPT}
	@echo "------SHARED TESTS------" >> results3b.txt
	make  --no-print-directory small >> results3b.txt
	make --no-print-directory medium >> results3b.txt
	make --no-print-directory large >> results3b.txt

test_dynamic: shared
	$(CC) -fPIC -c mylibrary.c
	$(CC) -shared mylibrary.o -o libmylibrary.so
	$(CC) main.c -o main -ldl -D DYNAMIC_LIB -O${OPT}
	@echo "------DYNAMIC TESTS------" >> results3b.txt
	make --no-print-directory small >> results3b.txt
	make --no-print-directory medium >> results3b.txt
	make --no-print-directory large >> results3b.txt

small:
	@echo "\n########### SMALL TESTS ###########\n" >> results3b.txt

	@./main create_main_array 10 wc_files test1.txt test2.txt test1.txt test2.txt test1.txt test2.txt test1.txt \
 	test2.txt test1.txt test2.txt

	@echo "\n########### SMALL add remove repeat ###########\n" >> results3b.txt
	@./main create_main_array 9 wc_files \
	test1.txt test2.txt test3.txt test1.txt test2.txt \
	test1.txt test2.txt test3.txt test1.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 \
	remove_block 5 remove_block 6 remove_block 7 remove_block 8 remove_block 9

medium:
	@echo "\n########### MEDIUM TESTS ###########\n" >> results3b.txt
	@./main create_main_array 10 wc_files \
	test4.txt test5.txt test6.txt test4.txt test5.txt\
	test4.txt test5.txt test6.txt test4.txt test5.txt

	@echo "\n########### MEDIUM add remove repeat ###########\n" >> results3b.txt
	@./main create_main_array 8 wc_files \
	test4.txt test5.txt test6.txt \
	test4.txt test5.txt test6.txt \
	test4.txt test5.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 \
	remove_block 5 remove_block 6 remove_block 7

large:
	@echo "\n########### LARGE TESTS ###########\n" >> results3b.txt
	@./main create_main_array 3 wc_files test7.txt test8.txt test9.txt

	@echo "\n########### LARGE add remove repeat ###########\n" >> results3b.txt
	@./main create_main_array 10 wc_files \
	test7.txt test8.txt test9.txt \
	test7.txt test8.txt test9.txt \
	test7.txt test8.txt test9.txt test7.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5 \
	remove_block 6 remove_block 7 remove_block 8 remove_block 9

