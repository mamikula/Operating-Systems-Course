#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

#define LINE_LEN 70

int w, h;
int threads_no;
int** image;
int** negative_image;

enum options {
    NUMBERS,
    BLOCK
};

void* numbers_routine(void* arg);
void* block_routine(void* arg);

int main(int argc, char* argv[]){

    if (argc != 5){
        exit(1);
    }
    threads_no = atoi(argv[1]);
    char* input_filename = argv[3];
    char* output_filename = argv[4];
    int option;

    //
    FILE* file1 = fopen(input_filename, "r");
    if (file1 == NULL){
        exit(1);
    }

    char* buff = calloc(LINE_LEN, sizeof(char));

    fgets(buff, LINE_LEN, file1);
    fgets(buff, LINE_LEN, file1);
    //w h
    fgets(buff, LINE_LEN, file1);
    if(buff[0] != '#'){
        sscanf(buff, "%d %d\n", &w, &h);
    }

    //max value
    fgets(buff, LINE_LEN, file1);
    if(buff[0] != '#'){
        int max;
        sscanf(buff, "%d \n", &max);
    }

    image = calloc(h, sizeof(int *));
    for (int i = 0; i < h; i++) {
        image[i] = calloc(w, sizeof(int));
    }

    int p;
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            fscanf(file1, "%d", &p);
            image[i][j] = p;
        }
    }
    fclose(file1);
    //

    if (strcmp(argv[2], "numbers") == 0) {
        option = NUMBERS;
    } else if (strcmp(argv[2], "block") == 0) {
        option = BLOCK;
    } else {
        perror("Wrong option (try numbers or block)\n");
        return 2;
    }

    negative_image = calloc(h, sizeof(int *));
    for (int i = 0; i < h; i++) {
        negative_image[i] = calloc(w, sizeof(int));
    }

    pthread_t* th = calloc(threads_no, sizeof(pthread_t));
    int* th_idx = calloc(threads_no, sizeof(int));

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    for(int i = 0; i < threads_no; i++){
        th_idx[i] = i;
        if (option == NUMBERS){
            pthread_create(&th[i], NULL, &numbers_routine, &th_idx[i]);
        }else if (option == BLOCK){
            pthread_create(&th[i], NULL, &block_routine, &th_idx[i]);
        }
    }

    FILE* times_file = fopen("Times.txt", "a");
    fprintf(times_file, "Inverting image: %s ", input_filename);
    fprintf(times_file, "with %d threads ", threads_no);
    if(option == NUMBERS){
        fprintf(times_file, "using mode: NUMBERS\n");
    }else if(option == BLOCK){
        fprintf(times_file, "using mode: BLOCK\n");
    }

    for(int i = 0; i < threads_no; i++) {
        long* elapsed;
        pthread_join(th[i], (void **) &elapsed);
        printf("Thread %d returned %ld microseconds\n", i, *elapsed);
        fprintf(times_file, "Thread: %d returned %5lu microseconds\n", i, *elapsed);
    }

    gettimeofday(&stop, NULL);
    long* elapsed = malloc(sizeof(long));
    *elapsed = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf("Main thread took %5lu microseconds\n\n", *elapsed);
    fprintf(times_file, "Main thread took:  %5lu microseconds\n\n\n", *elapsed);

    //si
    FILE *file2 = fopen(output_filename, "w");
    if (file2 == NULL) {
        exit(1);
    }

    fprintf(file2, "P2\n");
    fprintf(file2, "%d %d\n", w, h);
    fprintf(file2, "255\n");

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            fprintf(file2, "%d ", negative_image[i][j]);
        }
        fprintf(file2, "\n");
    }
    fclose(file2);
    fclose(times_file);

    for (int i = 0; i < h; i++) {
        free(image[i]);
        free(negative_image[i]);
    }
    free(image);
    free(negative_image);
    return 0;
}

void* numbers_routine(void* arg){
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    int idx = *((int *) arg);
    int from = 256 / threads_no * idx;
    int to;
    if(idx != threads_no - 1) to = 256 / threads_no * (idx + 1);
    else to = 256;

    int p;
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            p = image[i][j];
            if (p >= from && p < to){
                negative_image[i][j] = 255 - p;
            }
        }
    }

    gettimeofday(&stop, NULL);
    long* elapsed = malloc(sizeof(long));
    *elapsed = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    pthread_exit(elapsed);
}

void* block_routine(void* arg) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    int index = *((int *) arg);
    int x_from = (index) * ceil(w / threads_no);
    int x_to;
    if(index != threads_no - 1) x_to = (index + 1)* ceil(w / threads_no) - 1;
    else x_to = w - 1;

    int p;
    for (int i = 0; i < h; i++){
        for (int j = x_from; j <= x_to; j++){
            p = image[i][j];
            negative_image[i][j] = 255 - p;
        }
    }

    gettimeofday(&stop, NULL);
    long *elapsed = malloc(sizeof(long));
    *elapsed = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    pthread_exit(elapsed);
}