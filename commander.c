#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "errdefs.h"

int main(){
  int pipeway[2];
  int pMannagerId = 0;
  if(pipe(pipeway) == -1){
    perror("erro na criacao do Pipe");
    exit(PIPE_ERR);
  }

  if((pMannagerId = fork()) == -1){
    perror("Fork");
    exit(FORK_ERR);
  }

  if(pMannagerId == 0){
    char *string;
    string = (char*) malloc(50 * sizeof(char));
    fgets(string, 50, stdin);
    printf("%ld\n", strlen(string));
  }

  wait(0);
}
