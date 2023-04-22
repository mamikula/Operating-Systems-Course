#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>


long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms, FILE* file, int rounds){
    printf("\nEXECUTION TIME\n");
    printf("real %Lf\n", time_sec(clock_end - clock_start)/rounds);
    printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime)/rounds);
    printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime)/rounds);
    fprintf(file, "\nEXECUTION TIME\n");
    fprintf(file, "real %Lf\n", time_sec(clock_end - clock_start)/rounds);
    fprintf(file, "user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime)/rounds);
    fprintf(file, "sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime)/rounds);
}


void lib_find_char(char symbol, char *filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        printf("Cannot open file");
        exit(1);
    }
    int chars = 0;
    int lines = 0;
    int found;


    fseek(file, 0, SEEK_END);
    const int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char buffer[size];

    int i = 0;

    fread(&buffer, sizeof(char), size, file);

    while(i < size) {
        if (buffer[i] == symbol) {
            chars++;
            found = 1;
        }
        if (buffer[i] == '\n') {
            if (found == 1) lines++;
            found = 0;
        }
        i++;
    }
    if(found == 1) lines++;
    fclose(file);
}

void sys_find_char(char symbol, char *filename){
    int file = open(filename, O_RDONLY);
    int chars = 0;
    int lines = 0;
    int found;
    int i = 0;

    const int size = lseek(file, 0, SEEK_END);
    lseek(file, 0, SEEK_SET);
    char buffer[size];

    read(file, buffer, size);

    while(i < size) {
        if (buffer[i] == symbol) {
            chars++;
            found = 1;
        }
        if (buffer[i] == '\n') {
            if (found == 1) lines++;
            found = 0;
        }
        i++;
    }
    if(found == 1) lines++;
    close(file);
}


int main(int argc, char *argv[]) {

    FILE* result_file = fopen("pomiar_zad_2.txt", "w");
    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;
    struct tms start_tms2;
    struct tms end_tms2;
    clock_t clock_start2;
    clock_t clock_end2;
    int rounds = 50;

    if (argc == 3){
        char symbol = *(argv[1]);
        char *filename = argv[2];

        clock_start = times(&start_tms);
        for(int i = 0; i < rounds; i++) {
            lib_find_char(symbol, filename);
        }
        clock_end = times(&end_tms);

        clock_start2 = times(&start_tms2);
        for(int i = 0; i < rounds; i++) {
            sys_find_char(symbol, filename);
        }
        clock_end2 = times(&end_tms2);
    }

    else {
        printf("Wrong number of arguments!\n");
        return 1;
    }

    fprintf(result_file, "\n------LIB-----");
    printf("\n------LIB-----");
    print_res(clock_start, clock_end, start_tms, end_tms,result_file, rounds);

    fprintf(result_file, "\n------SYS-----");
    printf("\n------SYS-----");
    print_res(clock_start2, clock_end2, start_tms2, end_tms2, result_file, rounds);

    return 0;
}