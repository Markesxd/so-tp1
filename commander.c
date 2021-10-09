#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
  int pipeway[2];
  int pMannagerId = 0;
  if(pipe(pipeway) == -1){
    perror("erro na criacao do Pipe");
    return 1;
  }

  if((pMannagerId = fork()) == -1){
    perror("Erro na criacao do fork\n");
    return 2;
  }

  if(pMannagerId == 0){
    char *string;
    string = (char*) malloc(50 * sizeof(char));
    fgets(string, 50, stdin);
    printf("%d\n", strlen(string));
  }

  wait(0);
}
