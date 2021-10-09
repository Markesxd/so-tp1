#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


char* slice(char *string, int start, int end){
  char *buffer;
  int j = 0;

  buffer = (char*) malloc((end - start) * sizeof(char));
  for(int i = start; i < end; i++){
    buffer[j++] = string[i];
  }

  return buffer;
}

int getCommandNum(char *string){
  return atoi(slice(string, 2, strlen(string)));
}


int main(){
    char **program;
    int size = 0;
    int value = 0;
    int pid = -1;
    int pipeway[2];

    if(pipe(pipeway) == -1){
      perror("pipe");
      exit(1);
    }


    if(pid == 0){
      close(pipeway[1]);
      read(pipeway[0], &size, sizeof(int));
      read(pipeway[0], program, size * sizeof(char*));
    } else {
      close(pipeway[0]);

      program = (char**) malloc(sizeof(char*));

      while(1){
        char *command;
        command = (char*) malloc(10 * sizeof(char));
        fgets(command, 10, stdin);
        if(command[0] == 'E') break;
        program[size] = command;
        size++;
        program = (char**) realloc(program, sizeof(char*) * size + 1);
      }
    }


    for(int i = 0; i < size; i++){
      switch(program[i][0]){
        case 'S':
          value = getCommandNum(program[i]);
          break;
        case 'A':
          value += getCommandNum(program[i]);
          break;
        case 'D':
          value -= getCommandNum(program[i]);
          break;
        case 'F':
          if((pid = fork()) == -1){
            perror("fork");
            exit(2);
          }
          int resize = size - i;
          write(pipeway[1], &resize, sizeof(int));
          // write(pipeway[1], )
          size = i + getCommandNum(program[i]);
          break;
      }
    }

    printf("value: %d\n", value);
    wait(0);
}
