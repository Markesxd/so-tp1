#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "errdefs.h"
#include "fila.h"
#include "manager.h"

int main(){
  int processes = 1;
  int Time = 0;
  CPU cpu;
  PCBTable *pcb;
  stateList *ready, *blocked, executing;

  cpu = cpuInit();
  pcb = pcbInit();
  ready = initializeList();
  blocked = initializeList();
  executing.pid = 0;

  pcb[0].program = readProgram("./init.txt");
  pcb[0].state = RUNNING;

  char command = 'S';
  while(command != 'T'){
    int id = -1;
    int state = -1;
    scanf("%c", &command);
    printf("%c\n", command);
    switch(command){
      case 'Q':
        id = executing.pid;
        id = pcbSerch(id, pcb, processes);
        cpu = processExchange(pcb[id]);
        state = execute(&cpu);
        pcb[id] = save(cpu);
        pcb[id].state = state;
        Time++;
        //escalonamento
        break;
      default:
        printf("command malformed\n");
    }
  }
  printf("value in cpu: %d\n", cpu.value);

  wait(0);
}

char* slice(char *string, int start, int end){
  char *buffer;
  int j = 0;

  buffer = (char*) malloc((end - start) * sizeof(char));
  for(int i = start; i <= end; i++){
    buffer[j++] = string[i];
  }

  return buffer;
}

int getInstNum(char *string){
  return atoi(slice(string, 2, strlen(string)));
}

char** readProgram(char *path){
  int size = 0;
  FILE *f;

  f = fopen(path, "r");
  if(f == NULL){
    perror("Failed to read file");
    exit(READF_ERR);
  }

  char **program = malloc(sizeof(char**));

  while(1){
    char *command;
    command = (char*) malloc(MAX_INSTRUCTION_LEN * sizeof(char));
    fgets(command, MAX_INSTRUCTION_LEN, f);
    command[strlen(command) - 1] = '\0';
    program = (char**) realloc(program, ++size * sizeof(char**));
    program[size - 1] = command;
    if(command[0] == 'E') {
      break;
    };
  }

  if(fclose(f) == EOF){
    perror("read file");
    exit(READF_ERR);
  }

  return program;
}

PCBTable* pcbInit(){
  PCBTable *pcb;
  pcb = (PCBTable*) malloc(sizeof(PCBTable));
  pcb[0].pid = 0;
  pcb[0].ppid = -1;
  pcb[0].counter = 0;
  pcb[0].program = NULL;
  pcb[0].value = 0;
  pcb[0].priority = 0;
  pcb[0].state = WAITING;
  pcb[0].timeStart = 0;
  pcb[0].cpuUsage = 0;
  return pcb;
}

int pcbSerch(int pid,PCBTable *pcb, int size){
  for(int i = 0; i < size; i++){
    if(pcb[i].pid == pid) return i;
  }
}

CPU cpuInit(){
  CPU cpu;
  cpu.program = NULL;
  cpu.counter = 0;
  cpu.value = 0;
  cpu.execTime = 0;
  return cpu;
}

CPU processExchange(PCBTable pcb){
  CPU cpu;
  cpu.program = pcb.program;
  cpu.counter = pcb.counter;
  cpu.value = pcb.value;
  cpu.execTime = pcb.cpuUsage;
  return cpu;
}

int execute(CPU *cpu){
  char* instruction = cpu->program[cpu->counter];
  switch(instruction[0]){
    case 'S':
      cpu->value = getInstNum(instruction);
      return RUNNING;
    case 'A':
      cpu->value += getInstNum(instruction);
      return RUNNING;
    case 'D':
      cpu->value -= getInstNum(instruction);
      return RUNNING;
    case 'B':
      return BLOCKED;
    case 'E':
      return TERMINATED;
    case 'F':
      return RUNNING;
    case 'R':
      return RUNNING;
  }
}

PCBTable save(CPU cpu){
  PCBTable pcb;
  pcb.counter = cpu.counter + 1;
  pcb.value = cpu.value;
  pcb.cpuUsage = cpu.execTime + 1;
}
