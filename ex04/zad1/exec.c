#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Wrong number of aruments!");
        return -1;
    }

    if(strcmp(argv[1], "ignore") == 0){
        printf("\n-----------------IGNORE TEST-----------------\n");

        signal(SIGUSR1, SIG_IGN);

        printf("Raising signal...\n");
        raise(SIGUSR1);
        execl("./child", "./child", argv[1], NULL);
        printf("You couldn't see this line, execl() problem!");

    }

    else if(strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0) {
        if(strcmp(argv[1], "mask") == 0){
            printf("\n-----------------MASK TEST-----------------\n");
        }

        else if(strcmp(argv[1], "pending") == 0){
            printf("\n-----------------PENDING TEST-----------------\n");
        }

        sigset_t new_mask;
        sigset_t old_mask;
        sigemptyset(&new_mask);
        sigaddset(&new_mask, SIGUSR1);

        if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
            perror("Something wrong happend with blocking signal");
        }

        printf("Raising signal...\n");
        raise(SIGUSR1);

        if (sigismember(&new_mask, SIGUSR1) == 1) printf("Signal pending\n");
        else printf("Signal NOT pending\n");

        printf("Calling exec...\n");
        execl("./child", "./child", argv[1], NULL);
        printf("You couldn't see this line, execl() problem!");
    }

    return 0;
}