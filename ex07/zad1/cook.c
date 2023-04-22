#include "header.h"

void put_pizza_in_oven();

int take_out_pizza();

void place_pizza_on_table();

void put_pizza_in_oven(oven* ov, int type){
    ov->tab[ov->empty_slot] = type;
    ov->empty_slot++;
    ov->empty_slot = ov->empty_slot % OVEN_SIZE;
    ov->pizza_counter++;
}

int take_out_pizza(oven* ov){
    int type = ov->tab[ov->take_out_idx];
    ov->tab[ov->take_out_idx] = -1;
    ov->take_out_idx++;
    ov->take_out_idx = ov->take_out_idx % OVEN_SIZE;
    ov->pizza_counter--;
    return type;
}

void place_pizza_on_table(table* tb, int type){
    tb->tab[tb->empty_slot] = type;
    tb->empty_slot++;
    tb->empty_slot = tb->empty_slot % OVEN_SIZE;
    tb->pizza_counter++;
}

int main(){

    int sem_id = get_sem_id();
    int shm_oven_id = get_shm_id(OVEN_PATH, OVEN_ID);
    int shm_table_id = get_shm_id(TABLE_PATH, TABLE_ID);

    oven* ov = shmat(shm_oven_id, NULL, 0);
    table* tb = shmat(shm_table_id, NULL, 0);

    int type;

    srand(getpid());
    while(true){

        type = rand() % 10;
        printf("[COOK]  (pid: %d timestamp: %s) Przygotowuje pizze: %d\n", getpid(), get_time(), type);
        sleep(PREPARATION_TIME);

        lock_sem(sem_id, FULL_OVEN_SEM);
        lock_sem(sem_id, OVEN_SEM);

        put_pizza_in_oven(ov, type);
        printf("[COOK]  (pid: %d timestamp: %s) Dodałem pizze: %d. Liczba pizz w piecu: %d.\n", getpid(), get_time(), type, ov->pizza_counter);

        unlock_sem(sem_id, OVEN_SEM);
        sleep(BAKING_TIME);
        lock_sem(sem_id, OVEN_SEM);

        type = take_out_pizza(ov);
        printf("[COOK]  (pid: %d timestamp: %s) Wyjalem pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n", getpid(), get_time(), type, ov->pizza_counter, tb->pizza_counter);

        unlock_sem(sem_id, OVEN_SEM);
        unlock_sem(sem_id, FULL_OVEN_SEM);

        lock_sem(sem_id, FULL_TABLE_SEM);
        lock_sem(sem_id, TABLE_SEM);

        place_pizza_on_table(tb, type);
        printf("[COOK]  (pid: %d timestamp: %s) Umieszczam pizze [%d] na stole. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n", getpid(), get_time(), type, ov->pizza_counter, tb->pizza_counter);

        unlock_sem(sem_id, TABLE_SEM);
        unlock_sem(sem_id, EMPTY_TABLE_SEM);
    }
}