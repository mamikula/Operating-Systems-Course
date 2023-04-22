#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mylibrary.h"
#include <sys/times.h>
#include <unistd.h>

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_time(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms, char* operation){
    printf("%s\n ------\n", operation);
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));

}


int main(int argc, char *argv[]){
    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;

    struct tms start_tms2;
    struct tms end_tms2;
    clock_t clock_start_fun;
    clock_t clock_end_fun;

    clock_start = times(&start_tms);

    struct MainArray *arr = NULL;

    for(int i = 1; i < argc; i++){

        if (strcmp(argv[i], "create_main_array") == 0) {

            clock_start_fun = times(&start_tms2);
            int size = atoi(argv[++i]);
            arr = create_main_array(size);
            clock_end_fun = times(&end_tms2);
            print_time(clock_start_fun, clock_end_fun, start_tms2, end_tms2, "create_main_array TIME");
        }

        else if(strcmp(argv[i], "wc_files") == 0){


            char* file_name = (char*)calloc(strlen(argv[i]), sizeof(char));

            while(i < argc - 1 && strchr(argv[++i],'.')){
                clock_start_fun = times(&start_tms2);

                int file_len = strchr(argv[i], '.') - argv[i];
                for(int a = 0; a < file_len + 4; a++){
                    file_name[a] = argv[i][a];
                }
                wc_files(file_name);
                clock_end_fun = times(&end_tms2);
                print_time(clock_start_fun, clock_end_fun, start_tms2, end_tms2, "wc_files TIME");


                clock_start_fun = times(&start_tms2);
                block_operation(arr);
                clock_end_fun = times(&end_tms2);
                print_time(clock_start_fun, clock_end_fun, start_tms2, end_tms2, "block_operation TIME");

            }
            if(i != argc - 1) i--;
        }
        else if (strcmp(argv[i], "delete_block") == 0){
            clock_start_fun = times(&start_tms2);
            int id = atoi(argv[++i]);
            delete_block(arr, id);
            clock_end_fun = times(&end_tms2);
            print_time(clock_start_fun, clock_end_fun, start_tms2, end_tms2, "delete_block TIMn");

        }

    }
    clock_end = times(&end_tms);
    print_time(clock_start, clock_end, start_tms, end_tms, "TOTAL EXECUTION TIME");
    return 0;
}