#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define COMMAND_LEN 20

int main(){
  FILE *init;
  init = fopen("./init.txt", "r");
  char** program;
  int size = 0;
  int pid = -1;

  program = (char**) malloc(sizeof(char*));

  while(1){
    char *command;
    command = (char*) malloc(COMMAND_LEN * sizeof(char));
    fgets(command, COMMAND_LEN, init);
    program[size++] = command;
    if(command[0] == 'E') break;
    program = (char**) realloc(program, sizeof(char*) * size + 1);
  }
  if(fclose(init) == -1){
    perror("read file");
    exit(1);
  }

  if((pid = fork()) == -1){
    perror("fork");
    exit(2);
  }

  return 0;
}
