#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "errdefs.h"

#define COMMAND_LEN 20

int main(){
  char** program;
  int size = 0;
  int pid = -1;
  int pipeway[2];

  program = (char**) malloc(sizeof(char*));



  if(pipe(pipeway) == -1){
    perror("Pipe");
    exit(PIPE_ERR);
  }

    // close(pipeway[0]);
    FILE *init;
    init = fopen("./init.txt", "r");
    if(init == NULL){
      perror("Failed to read file");
      exit(READF_ERR);
    }

    while(1){
      char *command;
      command = (char*) malloc(COMMAND_LEN * sizeof(char));
      fgets(command, COMMAND_LEN, init);
      puts(command);
      program[size++] = command;
      if(command[0] == 'E') break;
      program = (char**) realloc(program, sizeof(char*) * size + 1);
    }

    write(pipeway[1], &size, sizeof(int));
    for(int i = 0; i < size; i++){
      write(pipeway[1], program[i], COMMAND_LEN);
    }

    if(fclose(init) == EOF){
      perror("read file");
      exit(READF_ERR);
    }

    if((pid = fork()) == -1){
      perror("fork");
      exit(FORK_ERR);
    }
    if(pid == 0){
      close(pipeway[1]);
      if(dup2(pipeway[0], STDIN_FILENO) == -1){
        perror("dup");
        exit(DUP_ERR);
      }
      close(pipeway[0]);
      execl("./p_simulado", "./p_simulado", NULL);
    }

  wait(0);
}
