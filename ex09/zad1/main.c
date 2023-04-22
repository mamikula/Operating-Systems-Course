#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define PRESENTS_TO_DELIVER 3
#define NUMBER_OF_REINDEERS 9
#define VACATION_FROM 5
#define VACATION_TO 10
#define DELIVERY_FROM 2
#define DELIVERY_TO 4

pthread_mutex_t reindeers_station = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reindeer_in_station = PTHREAD_COND_INITIALIZER;
pthread_cond_t delivering = PTHREAD_COND_INITIALIZER;

int reindeers_waiting = 0;
int delivered_presents = 0;

bool santa_delivering_presents = false;
bool all_presents_delivered = false;

pthread_t santa_thread;
pthread_t *reindeers;
int *reindeer_id;

typedef enum{
    VACATION,
    DELIVERY
} time_type;

int random_time(time_type type);

void* reindeer(void* arg);

void* santa(void* arg);

int main(int argc, char** argv){

    pthread_create(&santa_thread, NULL, santa, NULL);
    reindeers = calloc(NUMBER_OF_REINDEERS, sizeof (pthread_t));
    
    reindeer_id = calloc(NUMBER_OF_REINDEERS, sizeof(int));
    for (int i = 0; i < NUMBER_OF_REINDEERS; i++) {
        reindeer_id[i] = i+1;
        pthread_create(&reindeers[i], NULL,reindeer, &reindeer_id[i]);
    }

    pthread_join(santa_thread, NULL);
    for (int i = 0; i < NUMBER_OF_REINDEERS; i++) {
        pthread_join(reindeers[i], NULL);
    }

    free(reindeers);
    free(reindeer_id);

    return 0;
}

int random_time(time_type type){
    if(type == VACATION) return VACATION_FROM + (rand() % (VACATION_TO - VACATION_FROM));
    else return DELIVERY_FROM + (rand() % (DELIVERY_TO - DELIVERY_FROM));
}

void* reindeer(void* arg) {
    int *id = arg;
    while (!all_presents_delivered) {
        sleep(random_time(VACATION));

        pthread_mutex_lock(&reindeers_station);
        reindeers_waiting += 1;

        if (reindeers_waiting == 9) {
            printf("Renifer: wybudzam mikołaja, ID:%d\n", *id);
            pthread_cond_signal(&reindeer_in_station);
        } else {
            printf("Renifer: czeka %d reniferów na mikołaja, ID:%d\n", reindeers_waiting, *id);
        }

        // czekanie na wszystkie renifery
        while (!santa_delivering_presents) {
            pthread_cond_wait(&delivering, &reindeers_station);
        }

        reindeers_waiting -= 1;

        pthread_mutex_unlock(&reindeers_station);
        sleep(random_time(DELIVERY));

/*        pthread_mutex_unlock(&reindeers_station);
//        sleep(random_time(DELIVERY));
//
//        pthread_mutex_lock(&reindeers_station);
//        reindeers_waiting -= 1;
//        pthread_mutex_unlock(&reindeers_station);
//        sleep(random_time(DELIVERY));
*/

    }
    return id;
}

void* santa(void* arg) {
    while (delivered_presents < PRESENTS_TO_DELIVER) {
        pthread_mutex_lock(&reindeers_station);

        while (reindeers_waiting < NUMBER_OF_REINDEERS) {
            pthread_cond_wait(&reindeer_in_station, &reindeers_station);
        }

        printf("Mikołaj: budzę się\n");
        santa_delivering_presents = true;
        printf("Mikołaj: dostarczam zabawki %d\n", delivered_presents + 1);

        delivered_presents += 1;
        if(delivered_presents == PRESENTS_TO_DELIVER) all_presents_delivered = true;
        pthread_cond_broadcast(&delivering);

        pthread_mutex_unlock(&reindeers_station);

        sleep(random_time(DELIVERY));

        pthread_mutex_lock(&reindeers_station);
        printf("Mikołaj: zasypiam\n");
        santa_delivering_presents = false;
        pthread_mutex_unlock(&reindeers_station);
    }
    return 0;
}










// http://jedrzej.ulasiewicz.staff.ict.pwr.wroc.pl/SystemyCzasuRzeczywistego/wyklad/W%B9tki25.pdf