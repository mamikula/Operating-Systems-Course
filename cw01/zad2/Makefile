CC = gcc
CFLAGS = -Wall -g

static:
	$(CC) $(CFLAGS) -c mylibrary.c -o mylibrary.o
	ar rcs libmylibrary.a mylibrary.o
	$(CC) -c main.c
	$(CC) main.o libmylibrary.a -o main


clean:
	rm -rf raport2.txt tmpf.txt *.o *.a *.so main

test_static: static
	@echo raport2.txt
	make  --no-print-directory small >> raport2.txt
	make --no-print-directory medium >> raport2.txt
	make --no-print-directory large >> raport2.txt

small:
	@echo "\n########### SMALL TESTS ###########\n" >> raport2.txt
	@./main create_main_array 10 wc_files test1.txt test2.txt test1.txt test2.txt test1.txt test2.txt test1.txt \
 	test2.txt test1.txt test2.txt

	@echo "\n########### SMALL add remove repeat ###########\n" >> raport2.txt
	@./main create_main_array 9 wc_files \
	test1.txt test2.txt test3.txt test1.txt test2.txt \
	test1.txt test2.txt test3.txt test1.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 \
	remove_block 5 remove_block 6 remove_block 7 remove_block 8 remove_block 9


medium:
	@echo "\n########### MEDIUM TESTS ###########\n" >> raport2.txt
	@./main create_main_array 10 wc_files \
	test4.txt test5.txt test6.txt test4.txt test5.txt\
	test4.txt test5.txt test6.txt test4.txt test5.txt

	@echo "\n########### MEDIUM add remove repeat ###########\n" >> raport2.txt
	@./main create_main_array 8 wc_files \
	test4.txt test5.txt test6.txt \
	test4.txt test5.txt test6.txt \
	test4.txt test5.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 \
	remove_block 5 remove_block 6 remove_block 7


large:
	@echo "\n########### LARGE TESTS ###########\n" >> raport2.txt
	@./main create_main_array 3 wc_files test7.txt test8.txt test9.txt

	@echo "\n########### LARGE add remove repeat ###########\n" >> raport2.txt
	@./main create_main_array 10 wc_files \
	test7.txt test8.txt test9.txt \
	test7.txt test8.txt test9.txt \
	test7.txt test8.txt test9.txt test7.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5 \
	remove_block 6 remove_block 7 remove_block 8 remove_block 9

