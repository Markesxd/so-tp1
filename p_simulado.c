#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int getCommandNum(char *string){
  for(int i = 0; i < strlen(string); i++){
    
  }
}

int main(){
    char **program;
    int size = 0;
    int value = 0;

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

    for(int i = 0; i < size; i++){
      switch(program[0]){
        case 'S':

      }
    }
}
