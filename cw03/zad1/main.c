#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* args[]){
    if(argc != 2) exit(1);

    int n = atoi(args[1]);
    pid_t pid;
    for(int i = 0; i < n; i++){
        pid = fork();
        if(pid == 0){
            printf("%d. CHILD PID: %d, parent PID: %d\n", i, getpid(), getppid());
            exit(0);
        }

    }
    for(int i = 0; i < n; i++) wait(NULL);


    return 0;
}