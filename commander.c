#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "errdefs.h"

int main(){
  int pipeway[2];
  int pMannagerId = -1;
  if(pipe(pipeway) == -1){
    perror("erro na criacao do Pipe");
    exit(PIPE_ERR);
  }

  if((pMannagerId = fork()) == -1){
    perror("Fork");
    exit(FORK_ERR);
  }

  if(pMannagerId == 0){
    close(pipeway[1]);
    dup2(pipeway[0], STDIN_FILENO);
    close(pipeway[0]);
    execl("./manager", "./manager", NULL);
  } else{
    clock_t start = clock() / CLOCKS_PER_SEC;
    char command = 'a';
    while(1){
      scanf("%c", &command);
      printf("%c", command);
      if(command == 'T') break;

      while(start + 1 < clock() / CLOCKS_PER_SEC);
      start++;
    }
  }

  wait(0);
}
