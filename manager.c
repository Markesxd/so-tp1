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
  while(1){
    int id = -1;
    int state = -1;
    int counter = 0;
    scanf("%c", &command);
    if(command == 'T') break;
    printf("%c\n", command);
    switch(command){
      case 'Q':
        if(cpu.pid != executing.pid) cpu.cpuTime = 0;
        cpu.cpuTime++;
        cpu.pid = executing.pid;
        if(id == EMPTY_ERR){
          printf("No program ready to execute\n");
          break;
        }
        id = pcbSerch(cpu.id, pcb, processes);
        cpu = processExchange(pcb[id]);
        counter = cpu.counter;
        state = execute(&cpu);

        switch(state){
          case RUNNING:
            save(cpu, pcb + id);
            pcb[id].state = state;
            break;
          case BLOCKED:
            pcb[id].state = state;
            pushList(blocked, executing.pid);
            executing.pid = pullList(ready);
            break;
          case WAITING:
            pcb[id].state = state;
            pushList(ready, executing.pid);
            executing.pid = pullList(ready);
            break;
          case TERMINATED:
            pcb[id].state = state;
            executing.pid = pullList(ready);
            break;
          case FORKING:
            addPCB(&pcb, id, ++processes, counter, Time);
        }
        Time++;
        //escalonamento
        break;
      default:
        printf("command malformed\n");
    }
  }
  printf("value in pcb: %d\n", pcb[0].value);

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
      return FORKING;
    case 'R':
      return EXECING;
  }
}

void save(CPU cpu, PCBTable *pcb){
  pcb->counter = cpu.counter + 1;
  pcb->value = cpu.value;
  pcb->cpuUsage = cpu.execTime + 1;
}

int addPCB(PCBTable **pcb, int ppcb, int size, int counter, int time){
  *pcb = (PCBTable*) realloc(*pcb, size * sizeof(PCBTable));
   *pcb[size - 1].pid = size - 1;
   *pcb[size - 1].ppid = ppcb.pid;
   *pcb[size - 1].counter = counter;
   *pcb[size - 1].program = ppcb.program;
   *pcb[size - 1].value = ppcb.value;
   *pcb[size - 1].priority = ppcb.priority;
   *pcb[size - 1].state = WAITING;
   *pcb[size - 1].timeStart = time;
   *pcb[size - 1].cpuUsage = 0;
   return size - 1;
}
