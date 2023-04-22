#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void
sighandler(int sig, siginfo_t *info, void *ucontext)
{
    int siginf = info->si_value.sival_int;
    printf("SIGINFO recieved %d\n", siginf);
}



int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;
    action.sa_flags = SA_SIGINFO;
    //..........
    sigset_t sset;
    sigfillset(&sset);
    sigdelset(&sset, SIGUSR1);
    sigprocmask(SIG_SETMASK, &sset, NULL);

    sigaction(SIGUSR1, &action, NULL);

    int child = fork();
    if(child == 0) {

        //zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        //zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc
    }
    else {
        union sigval val;
        val.sival_int = atoi(argv[1]);
        int sig = atoi(argv[2]);
        sigqueue(child, sig, val);

        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]
    }

    return 0;
}
