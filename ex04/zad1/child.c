#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(int argc, char* argv[]) {
    printf("Child\n");
    if(strcmp(argv[1], "ignore") == 0){
        printf("Rising signal in child process...\n");
        raise(SIGUSR1);
    }
    else if(strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0) {
        // if "pending" mode then don't raise a signal in child process
        if (strcmp(argv[1], "mask") == 0) {
            printf("Rising signal in child process...\n");
            raise(SIGUSR1);
        }
        sigset_t new_mask; //Typ sigset_t sluzy do trzymania zbioru sygnalow
        sigpending(&new_mask); //pobiera zbior wstrzymywanych (przez maske blokowania) sygnalow procesu
        if (sigismember(&new_mask, SIGUSR1) == 1) printf("Signal pending [in child process]\n");
        else printf("Signal NOT pending [in child process]\n");
    }
    return 0;
}