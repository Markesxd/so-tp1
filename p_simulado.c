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

char** setChildProgram(char **program, int size,int start){
  char** childProgram = (char**) malloc((size - start) * sizeof(char*));
  int j = 0;
  for(int i = start; i < size; i++){
    childProgram[j++] = program[i];
  }

  return childProgram;
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

      program = (char**) malloc(sizeof(char*));

    for(int i = 0; i < size; i++){
      char *path;
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
        case 'B':
          // block
          break;
        case 'F':
          if((pid = fork()) == -1){
            perror("fork");
            exit(2);
          }
          if(pid == 0){
            program = setChildProgram(program, size, i + 1);
            size = size - i - 1;
            i = -1;
          } else{
            size = i + getCommandNum(program[i]) + 1;
          }
          break;
        case 'R':
          path = slice(program[i], 2, strlen(program[i]) - 1);
          if(execl(path, path, NULL) == -1){
            perror("exec");
            exit(3);
          }
          break;
        default:
         printf("malformed command at line %d\n", i + 1);
      }
    }

    printf("value: %d\n", value);
    wait(0);
}
