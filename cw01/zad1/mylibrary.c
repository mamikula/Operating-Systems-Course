#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mylibrary.h"

struct MainArray *create_main_array(int size){
    struct MainArray *arr = calloc(1, sizeof(struct MainArray));

    arr -> size = size;
    arr -> last_added = -1;
    arr -> blocks = calloc(size, sizeof(struct Block));
    printf("Array created succesfully\n");

    return arr;
}


void wc_files(char* file_name){
    FILE *file = fopen(file_name, "r");

    if(file == NULL){
        perror("Cannot open file");
        exit(1);
    }

    char *fn = (char*)calloc(1, 256);
    strcat(fn, "wc < ");
    strcat(fn, file_name);
    strcat(fn, " > tmpf.txt");
    printf("%s\n", fn);
    system(fn);
    free(fn);
}


int block_operation(struct MainArray* main_arr){

    FILE *tmpf = fopen("tmpf.txt", "r");
    if(tmpf == NULL) {
        printf("nie działa");
        exit(1);
    }
        struct Block *block = calloc(1, sizeof(struct Block));
        block->data =calloc(1, sizeof(tmpf));

        fgets(block->data, sizeof(tmpf), tmpf);
        main_arr->blocks[main_arr->last_added + 1] = block;
        main_arr->last_added++;
        printf("Added block at index %d\n", main_arr->last_added);
        fclose(tmpf);
        system("rm tmpf.txt");

    return main_arr->last_added;
}


void delete_block(struct MainArray* arr, int id){
    if(arr->blocks[id] == NULL) return;
    free(arr->blocks[id]);
    arr->blocks[id] = NULL;
    printf("Block removed");
}