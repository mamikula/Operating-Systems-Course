#include <stdlib.h>
#include <pwd.h>
#include <sys/time.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define ID 'R'
#define OVEN_ID 'O'
#define TABLE_ID 'T'
#define PERMISSIONS 0666
#define OVEN_PATH "oven"
#define TABLE_PATH "table"
#define OVEN_SIZE 5
#define TABLE_SIZE 5
#define OVEN_SEM 0
#define TABLE_SEM 1
#define FULL_OVEN_SEM 2
#define FULL_TABLE_SEM 3
#define EMPTY_TABLE_SEM 4
#define PREPARATION_TIME 1
#define BAKING_TIME 4
#define DELIVERY_TIME 4
#define RETURN_TIME 4

char* home_path(){
    char* path = getenv("HOME");
    if (path == NULL) {
        path = getpwuid(getuid())->pw_dir;
    }
    return path;
}

typedef struct{
    int tab[OVEN_SIZE];
    int pizza_counter;
    int empty_slot;
    int take_out_idx;
} oven;

typedef struct{
    int tab[TABLE_SIZE];
    int pizza_counter;
    int empty_slot;
    int take_out_idx;
} table;

void lock_sem(int sem_id, int sem_num){
    struct sembuf sem = {
            .sem_num = sem_num,
            .sem_op = -1
    };
    if (semop(sem_id, &sem, 1) == -1){
        printf("Semaphore locking error\n");
        exit(1);
    }
}

void unlock_sem(int sem_id, int sem_num) {
    struct sembuf sem = {
            .sem_num = sem_num,
            .sem_op = 1
    };
    if (semop(sem_id, &sem, 1) == -1) {
        printf("Semaphore unlocking error\n");
        exit(1);
    }
}

int get_sem_id(){
    key_t key = ftok(home_path(), ID);
    int sem_id;
    if((sem_id = semget(key, 5,0)) == -1){
        printf("Semaphore connecting error\n");
        exit(1);
    }
    return sem_id;
}

int get_shm_id(char* path, int id){
    key_t key_o = ftok(path, id);
    int shm_id;
    if ((shm_id = shmget(key_o, sizeof(oven), 0)) == -1){
        printf("%s", path);
        printf( " id getting error\n");
        exit(1);
    }
    return shm_id;
}

char* get_time(){
    struct timeval time_var;
    gettimeofday(&time_var, NULL);
    int millisec = time_var.tv_usec / 1000;
    char* buffer = calloc(80, sizeof(char));
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&time_var.tv_sec));
    char* current_time = calloc(84, sizeof(char));
    sprintf(current_time, "%s:%03d", buffer, millisec);
    return current_time;
}


