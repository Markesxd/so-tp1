#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "fila.h"
#include "errdefs.h"

#define COMMAND_LEN 25

typedef struct {
  int value;
  int active;
  int currentInstruction;
  int end;
} p_simulated;

char* slice(char *string, int start, int end){
  char *buffer;
  int j = 0;

  buffer = (char*) malloc((end - start) * sizeof(char));
  for(int i = start; i <= end; i++){
    buffer[j++] = string[i];
  }

  return buffer;
}

int getCommandNum(char *string){
  return atoi(slice(string, 2, strlen(string)));
}

int main(){
  char** program;
  int size = 0;
  int pid = -1;
  int pipeway[2];
  p_simulated *processList;
  fila *blockedList;
  int processes = 1;
  int blockedProcesses = 0;

  processList = (p_simulated*) malloc(sizeof(p_simulated));
  processList[0].value = 0;
  processList[0].active = 1;
  processList[0].currentInstruction = 0;

  blockedList = inicializaFila();
  program = (char**) malloc(sizeof(char*));

  if(pipe(pipeway) == -1){
    perror("Pipe");
    exit(PIPE_ERR);
  }

  //read file init with first simulated process program
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
    command[strlen(command) - 1] = '\0';
    puts(command);
    program = (char**) realloc(program, ++size * sizeof(char**));
    program[size - 1] = command;
    if(command[0] == 'E') {
      processList[0].end = size;
      break;
    };
  }

  if(fclose(init) == EOF){
    perror("read file");
    exit(READF_ERR);
  }

  //run processes
  {int i = 0;
    while(1){
    if(processList[i].currentInstruction == processList[i].end){
      processList[i].active = 0;
      printf("value of process %d: %d\n", i, processList[i].value);
      blockedProcesses++;
    }
    char *path;
    int current = processList[i].currentInstruction;
    processList[i].currentInstruction++;
    if(processList[i].active){
      switch(program[current][0]){
        case 'S':
        processList[i].value = getCommandNum(program[current]);
        break;
        case 'A':
        processList[i].value += getCommandNum(program[current]);
        break;
        case 'D':
        processList[i].value -= getCommandNum(program[current]);
        break;
        case 'B':
        processList[i].active = 0;
        blockedProcesses++;
        enfileiraFila(blockedList, i);
        break;
        case 'F':
        processList[i].end = current + getCommandNum(program[current]) + 1;
        processList = (p_simulated*) realloc(processList, ++processes * sizeof(p_simulated));
        processList[processes - 1].value = 0;
        processList[processes - 1].active = 1;
        processList[processes - 1].currentInstruction = current + 1;
        break;
        case 'R':
        path = slice(program[current], 2, strlen(program[current]));
        if((pid = fork()) == -1){
          perror("fork");
          exit(FORK_ERR);
        }
        if(pid == 0){
          if(execl(path, path, NULL) == -1){
            perror("exec");
            exit(EXEC_ERR);
          }
        }
        break;
        case 'E':
        processList[i].active = 0;
        blockedProcesses++;
        printf("value of process %d: %d\n", i, processList[i].value);
        break;
        default:
        printf("malformed command at %d line\n", current);
      }
    }
    int flag = 0;
    for(int j = i + 1; j < processes; j++){
      if(processList[j].active){
        i = j;
        flag = 1;
      }
    }
    if(!flag){
      i = 0;
    }
    if(processes == blockedProcesses) break;
  }}

  imprimeFila(blockedList);
  wait(0);
}
