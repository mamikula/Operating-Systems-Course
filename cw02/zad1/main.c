#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

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


void lib_fun(char* read_filename, char* write_filename){

    FILE* read_file = fopen(read_filename,"r");
    FILE* write_file = fopen(write_filename,"w");

    fseek(read_file, 0, SEEK_END);
    const int size = ftell(read_file);
    fseek(read_file, 0, SEEK_SET);
    char buffer[size];

    int i = 0;
    int j = 0;

    fread(buffer,sizeof(char),size,read_file);
    char second_buffer[size];

    while(i < size){
        if(buffer[i] == 13 && buffer[i+1] == 10 && buffer[i + 2] == 13 && buffer[i + 3] == 10){
            i += 2;

        }
        else if(buffer[i] == 13 && buffer[i+1] == 10) {
            int n = i + 2;
            while (isspace(buffer[n]) != 0 && buffer[n] != 13 && buffer[n] != 10) {
                n++;
            }
            if (buffer[n] == 13 && buffer[n + 1] == 10) i = n;
            else if (buffer[n] == '\0') i = n;

            else {
                second_buffer[j] = buffer[i];
                i++;
                j++;
            }
        }
        else {
            second_buffer[j] = buffer[i];
            i++;
            j++;
        }
    }

    fwrite(second_buffer, sizeof(char), j, write_file);
    fclose(write_file);
    fclose(read_file);

}


void sys_fun(char* read_filename, char* write_filename){


    int read_file = open(read_filename, O_RDONLY);
    int write_file = open(write_filename,O_WRONLY);
    const int size = lseek(read_file, 0, SEEK_END);
    lseek(read_file, 0, SEEK_SET);
    char buffer[size];

    int i = 0;
    int j = 0;

    read(read_file, buffer, size);
    char second_buffer[size];

    while(i < size){
        if(buffer[i] == 13 && buffer[i+1] == 10 && buffer[i + 2] == 13 && buffer[i + 3] == 10){
            i += 2;

        }
        else if(buffer[i] == 13 && buffer[i+1] == 10) {
            int n = i + 2;
            while (isspace(buffer[n]) != 0 && buffer[n] != 13 && buffer[n] != 10) {
                n++;
            }
            if (buffer[n] == 13 && buffer[n + 1] == 10) i = n;
            else if (buffer[n] == '\0') i = n;

            else {
                second_buffer[j] = buffer[i];
                i++;
                j++;
            }
        }
        else {
            second_buffer[j] = buffer[i];
            i++;
            j++;
        }
    }
    write(write_file, second_buffer, j);
    close(write_file);
    close(read_file);
}


int main(int argc, char *argv[]) {
    char* read_filename = argv[1];
    char* write_filename = argv[2];

    if(read_filename == NULL){
        scanf("Podaj plik do odczytu: %s", read_filename);
    }

    if(write_filename == NULL){
        scanf("Podaj plik do zapisu: %s", write_filename);
    }

    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;
    struct tms start_tms2;
    struct tms end_tms2;
    clock_t clock_start2;
    clock_t clock_end2;
    FILE* result_file = fopen("pomiar_zad_1.txt", "w");
    int rounds = 40;

    clock_start = times(&start_tms);
    for(int i = 0; i < rounds; i++) {
        lib_fun(read_filename, write_filename);
    }
    clock_end = times(&end_tms);

    clock_start2 = times(&start_tms2);
    for(int i = 0; i < rounds; i++) {
        sys_fun(read_filename, write_filename);
    }

    clock_end2 = times(&end_tms2);

    fprintf(result_file, "\n------LIB-----");
    printf("\n------LIB-----");
    print_res(clock_start, clock_end, start_tms, end_tms,result_file, rounds);

    fprintf(result_file, "\n------SYS-----");
    printf("\n------SYS-----");
    print_res(clock_start2, clock_end2, start_tms2, end_tms2, result_file, rounds);


    return 0;
}