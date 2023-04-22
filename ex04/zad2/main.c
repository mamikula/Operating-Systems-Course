#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler_SIGINT(int sig, siginfo_t *info, void *ucontext){
    printf("\n\nReceived SIGINT signal (%d). PID: %d PPID: %d\n", info->si_signo, info->si_pid, getppid());
    printf("--------------- info ----------------\n");
    printf("Real user ID of sending process: %d\n", info->si_uid);
    printf("Signal code: %d\n", info->si_code);
    printf("User time consumed: %ld\n", info->si_utime);
    kill(info->si_pid, SIGINT);
}

void handler_SIGCHLD(int sig, siginfo_t *info, void *ucontext){
    printf("\n\nReceived SIGCHLD signal (%d). PID: %d PPID: %d\n", info->si_signo, info->si_pid, getppid());
    printf("--------------- info ----------------\n");
    printf("Real user ID of sending process: %d\n", info->si_uid);
    printf("Signal code: %d\n", info->si_code);
    printf("User time consumed: %ld\n", info->si_utime);
    exit(0);
}

void handler_SIGINT_as_child(int sig, siginfo_t *info, void *ucontext){
    printf("\n\nReceived SIGINT signal [as child] (%d). PID: %d PPID: %d\n", info->si_signo, info->si_pid, getppid());
    printf("--------------- info ----------------\n");
    printf("Real user ID of sending process: %d\n", info->si_uid);
    printf("Signal code: %d\n", info->si_code);
    printf("User time consumed: %ld\n", info->si_utime);
    exit(0);
}

void handler_NOCLDSTOP(int signum){
    printf("Received signal SIGCHLD (%d). PID: %d, PPID: %d\n", signum, getpid(), getppid());
}

void handler_RESETHAND(int signum){
    printf("Handler function here\n");
    printf("Received signal (%d). PID: %d, PPID: %d\n", signum, getpid(), getppid());
}

void test_NOCLDSTOP(){
    printf("\n\n\n---------------- TEST SA_NOCLDSTOP ----------------\n");

//    SA_NOCLDSTOP
//                  Jeśli   signum  jest  równe  SIGCHLD,  to  nie  są  odbierane  powiadomienia  o
//                  zatrzymaniu procesu-dziecka (np. gdy dziecko otrzyma jeden z SIGSTOP,  SIGTSTP,
//                  SIGTTIN  lub  SIGTTOU) ani o jego wznowieniu (np. po otrzymaniu SIGCONT).
//                  Flaga ta ma znaczenie tylko w przypadku  ustawiania  funkcji  obsługi
//                  sygnału SIGCHLD.

    struct sigaction act_SIGCHLD;
    sigemptyset(&act_SIGCHLD.sa_mask);
    act_SIGCHLD.sa_handler = &handler_NOCLDSTOP;

    sigaction(SIGCHLD, &act_SIGCHLD, NULL);

    // fork and stop child process without flag

    printf("\n~~~~ SA_NOCLDSTOP flag not set ~~~~\n\n");

    pid_t pid = fork();
    if(pid == 0){
        printf("Child process :%d\n: ", getpid());
//        while(1){};
    }
    else{
        printf("Parent process :%d\n: ", getpid());
        printf("Child proces stoppedm SIGCHLD should be sent\n");
        kill(pid, SIGSTOP);
        sleep(2);
    }
//-----------------------------------------------------------------------
    printf("\n~~~~   SA_NOCLDSTOP flag set   ~~~~\n\n");

    act_SIGCHLD.sa_flags = SA_NOCLDSTOP;

    if(pid == 0){
        printf("Child process :%d\n: ", getpid());
        while(1){}
    }
    else{
        printf("Parent process :%d\n: ", getpid());
        printf("Child proces stopped SIGCHLD should NOT be sent\n");
        kill(pid, SIGSTOP);
        sleep(2);
    }

}

void test_SININFO(){
    printf("\n\n\n---------------- TEST SA_SIGINFO ----------------\n");
//    SA_SIGINFO, powodująca, że podczas wystąpienia sygnału wywoływana
//    jest funkcja z pola sa_sigaction, a nie domyślna sa_handler.
    struct sigaction act_SIGINT;
    sigemptyset(&act_SIGINT.sa_mask);
    act_SIGINT.sa_sigaction = handler_SIGINT;
    act_SIGINT.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act_SIGINT, NULL);


    struct sigaction act_SIGCHLD;
    sigemptyset(&act_SIGCHLD.sa_mask);
    act_SIGCHLD.sa_sigaction = handler_SIGCHLD;
    act_SIGCHLD.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &act_SIGCHLD, NULL);


    pid_t pid = fork();
    if(pid == 0){
        signal(SIGINT, (__sighandler_t) &handler_SIGINT_as_child);

        // send signal to parent process
        kill(getppid(), SIGINT);
        pause();
    }
    else while(1){}

}

void test_RESETHAND(){
    printf("\n\n\n---------------- TEST SA_RESETHAND ----------------\n");

    struct sigaction act;
    act.sa_handler = handler_RESETHAND;
    sigemptyset(&act.sa_mask);
    sigaction(SIGUSR1, &act, NULL);
    printf("\n~~~~   SA_RESETHAND flag NO set   ~~~~\n\n");


    raise(SIGUSR1);
    sleep(1);
    raise(SIGUSR1);

    printf("\n~~~~   SA_RESETHAND flag set   ~~~~\n\n");
    act.sa_flags = SA_RESETHAND;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1);
    sleep(1);
    raise(SIGUSR1);
}

int main(){

    test_NOCLDSTOP();
    test_RESETHAND();
    test_SININFO();

    return 0;
}