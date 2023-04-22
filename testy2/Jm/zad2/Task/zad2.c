#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>


void licz(int x){
  printf("dla x=%d wynik x2 to %d\n",x , x*x); fflush(stdout);
  return;
}

int main (int lpar, char *tab[]){
  pid_t pid;
  int i;
  for(i = 1; i < 21; i++){
      pid = fork();
      if(pid== 0){
          licz(i);
          return 0;
      }
      else{
          while(wait(0) > 0) printf("PID procesu : %d\n", pid);
      }
  }


/* 1) wywolaj funkcje 'licz' dla parametrow od 1 do 20. Kazde wywolanie tej
   funkcji ma byc w innym procesie potomnym
   2) proces macierzysty ma wyswietlic pid wszystkich procesow potomnych i
   czekac na zakonczenie ich wszystkich*/



  return 0;
}
