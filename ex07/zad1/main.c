#include "header.h"

void create_new_oven();

void create_new_table();

void create_shar_mem_segment();

void create_semaphores();

void sigint_handler();

int shm_oven_id, shm_table_id, sem_id;

void create_new_oven(oven* ov){
    for (int i = 0; i < OVEN_SIZE; i++){
        ov->tab[i] = -1;
    }
    ov->pizza_counter = 0;
    ov->empty_slot = 0;
    ov->take_out_idx = 0;
}

void create_new_table(table* tb){
    for (int i = 0; i < OVEN_SIZE; i++){
        tb->tab[i] = -1;
    }
    tb->pizza_counter = 0;
    tb->empty_slot = 0;
    tb->take_out_idx = 0;
}

void create_shar_mem_segment(){
    key_t key_o, key_t;
    key_o = ftok(OVEN_PATH, OVEN_ID);
    key_t = ftok(TABLE_PATH, TABLE_ID);
    printf("o: %d, t: %d\n", key_o, key_t);
    shm_oven_id = shmget(key_o, sizeof(oven), IPC_CREAT | PERMISSIONS);
    shm_table_id = shmget(key_t, sizeof(table), IPC_CREAT | PERMISSIONS);

    oven *new_oven = shmat(shm_oven_id, NULL, 0);
    table *new_table = shmat(shm_table_id, NULL, 0);
    create_new_oven(new_oven);
    create_new_table(new_table);

    printf("Shared memory segment created successfuly\noven_id: %d, table_id: %d \n\n", shm_oven_id, shm_table_id);

}

void create_semaphores(){
    key_t key;
    key = ftok(home_path(), ID);
    sem_id = semget(key, 5, PERMISSIONS | IPC_CREAT);
    semctl(sem_id, OVEN_SEM, SETVAL, 1);
    semctl(sem_id, TABLE_SEM, SETVAL,1);
    semctl(sem_id, FULL_OVEN_SEM, SETVAL,OVEN_SIZE);
    semctl(sem_id, FULL_TABLE_SEM, SETVAL,TABLE_SIZE);
    semctl(sem_id, EMPTY_TABLE_SEM, SETVAL,0);
    printf("Semaphore set created successfuly\nsem_id: %d\n\n", sem_id);
}

void sigint_handler(int signum){
    semctl(sem_id, 0, IPC_RMID, NULL);
    shmctl(shm_oven_id, IPC_RMID, NULL);
    shmctl(shm_table_id, IPC_RMID, NULL);
}

int main(int argc, char* argv[]){

    if (argc != 3){
        printf("Wrong number of arguments\n");
        return -1;
    }

    signal(SIGINT, sigint_handler);

    int cooks = atoi(argv[1]);
    int suppliers = atoi(argv[2]);

    create_shar_mem_segment();
    create_semaphores();

    for (int i = 0; i < cooks; i++){
        pid_t pid = fork();
        if (pid == 0){
            execl("./cook", "./cook", NULL);
            printf("Execl() error\n");
        }
    }

    for (int i = 0; i < suppliers; i++){
        pid_t pid = fork();
        if (pid == 0){
            execl("./supplier", "./supplier", NULL);
            printf("Execl() error\n");
        }
    }

    for(int i = 0; i < cooks + suppliers; i++) wait(NULL);

    return 0;
}

