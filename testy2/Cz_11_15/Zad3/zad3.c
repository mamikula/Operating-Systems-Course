

/* pipe_ls_wc.c
   Program wykonuje instukcje potoku "ls | wc -l" 
   Uzupelnij wlasciwe instrukcje w miejscach *****
*/
#include <sys/wait.h>
#include <sys/types.h> 
#include <stdio.h>      
#include <stdlib.h>     
#include <unistd.h>     
#include <errno.h>      
#include <string.h>     

int
main(int argc, char *argv[])
{
    int pfd[2];
    //pfd[1] - zapis/write
    //pfd[0] - odczyt/read

    if (pipe(pfd) == -1)
        {
         fprintf(stderr, "pipe");
         exit(-1);
        }
    switch (fork()) {
    case -1:
         fprintf(stderr, "fork");
         exit(-1);

    case 0:
        if (close(pfd[0]) == -1)  //pfd[0] read
            {
             fprintf(stderr, "close 1");
             exit(-1);
           }


        if (pfd[1] != STDOUT_FILENO) {
            if (dup2(pfd[1], STDOUT_FILENO) == -1) //(write, STDOUT
                {
                  fprintf(stderr, "dup2 1");
                  exit(-1);
                }

            if (close(pfd[1]) == -1) //(
                {
                  fprintf(stderr, "close 2");
                   exit(-1);
                }
        }

        /***** Uruchom proces ls *****/
        execl("/user/bin/ls", "/user/bin/ls", "", NULL);
        exit(-1);

    default:
        break;
    }

    switch (fork()) {
    case -1:
           fprintf(stderr, "fork");
           exit(-1);
    case 0:
        if (close(pfd[1]) == -1)
            {
            fprintf(stderr, "close 3");
            exit(-1);
           }


      /***** uzyj dup2 z wlasciwymi argumentami *****/
      dup2(pfd[0], STDIN_FILENO);

       if (close(pfd[0]) == -1)
                {
                  fprintf(stderr, "close 4");
                  exit(-1);
                }



        /***** uruchom proces wc -l *****/

        execl("/user/bin/wc", "/user/bin/wc", "-l", NULL);
        exit(-1);

        default:
            break;

    }



    if (close(pfd[0]) == -1)
        {
                  fprintf(stderr, "close 5");
                   exit(-1);
        }


    if (close(pfd[1]) == -1)
        {
                  fprintf(stderr, "close 6");
                   exit(-1);
        }
 
   if (wait(NULL) == -1)
        {
            fprintf(stderr, "wait 1");
            exit(-1);
        }

    if (wait(NULL) == -1)
        {
            fprintf(stderr, "wait 2");
            exit(-1);
        }

    exit(EXIT_SUCCESS);
}
