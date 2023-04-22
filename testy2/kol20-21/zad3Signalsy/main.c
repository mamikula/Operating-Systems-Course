#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

//
//handlera napisać
void sighandler(int sig, siginfo_t *info, void *ucontext)
{
    int value = info->si_value.sival_int;
    if(sig == SIGUSR1){
        printf("Received signal: SIGUSR1. Value: %d\n", value);
    }
    else{
        printf("Received signal: SIGUSR2. Value: %d\n", value);
    }
}



int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;
    action.sa_flags = SA_SIGINFO;



    if (sigaction(SIGUSR1, &action, NULL) == -1)
    {
        fprintf(stderr, "Failed to setup handler for SIGUSR1\n");
    }

    if (sigaction(SIGUSR2, &action, NULL) == -1)
    {
        fprintf(stderr, "Failed to setup handler for SIGUSR1\n");
    }

    //-- zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1 i SIGUSR2 --
    // Blocking each signal in the set (the one in our case)


    //-- zdefiniuj obsluge SIGUSR1 i SIGUSR2 w taki sposob zeby proces potomny wydrukowal --
    //na konsole przekazana przez rodzica wraz z sygnalami SIGUSR1 i SIGUSR2 wartosci

    sigset_t unblock_set;
    sigfillset(&unblock_set);
    sigdelset(&unblock_set,SIGUSR1);
    sigdelset(&unblock_set,SIGUSR2);
    sigprocmask(SIG_SETMASK, &unblock_set, NULL);

    int child = fork();
    if(child == 0) sleep(1);
    else{
        union sigval val;
        val.sival_int = atoi(argv[1]);
        int sig = atoi(argv[2]);

        sigqueue(child, sig, val);

        int status = 0;
        wait(&status);
    }

    return 0;
}