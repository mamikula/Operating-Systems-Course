#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main (int lpar, char *tab[]){
  int w1;
  char buf[1024];
// 1) utworzyc potok nazwany 'potok1'
// 2) wyswietlac liczby otrzymywane z potoku
	mkfifo("potok1",S_IFIFO);
	
  while (1){
	FILE *pipeodb=fopen("potok1","r");
	fseek(pipeodb,0,SEEK_END);
	int size=ftell(pipeodb);
	fseek(pipeodb,0,SEEK_SET);
	fread(buf,sizeof(char),1024,pipeodb);
//
    printf("otrzymano: %s\n",buf);
    fflush (stdout);
  }

  return 0;
}
