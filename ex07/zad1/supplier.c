#include "header.h"

int deliver_pizza();

int deliver_pizza(table* tb){
    int type = tb->tab[tb->take_out_idx];
    tb->tab[tb->take_out_idx] = -1;
    tb->take_out_idx++;
    tb->take_out_idx = tb->take_out_idx % TABLE_SIZE;
    tb->pizza_counter--;
    return type;
}

int main(){

    int sem_id = get_sem_id();
    int shar_mem_table_id = get_shm_id(TABLE_PATH, TABLE_ID);
    table* tb = shmat(shar_mem_table_id, NULL, 0);
    int type;

    while(true) {
        lock_sem(sem_id, EMPTY_TABLE_SEM);
        lock_sem(sem_id, TABLE_SEM);

        type = deliver_pizza(tb);
        printf("[SUPP]  (pid: %d timestamp: %s) Pobieram pizze: %d. Liczba pizz na stole: %d.\n", getpid(), get_time(), type, tb->pizza_counter);

        unlock_sem(sem_id, TABLE_SEM);
        unlock_sem(sem_id, FULL_TABLE_SEM);

        sleep(DELIVERY_TIME);

        printf("[SUPP]  (pid: %d timestamp: %s) Dostarczam pizze: %d.\n", getpid(), get_time(), type);

        sleep(RETURN_TIME);
    }
}