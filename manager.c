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
  int lastP = -1;
  while(1){
    int id = -1;
    int state = -1;
    int counter = 0;
    scanf("%c", &command);
    if(command == '\n') continue;
    if(command == 'T'){
      reporter(pcb, processes, Time);
      break;
    }
    printf("%c\n", command);
    switch(command){
      case 'Q':
        printf("cpu id: %d\n", cpu.pid);
        printf("id: %d\n", executing.pid);
          if(lastP != executing.pid){
            cpu.execTime = 0;
          }
          lastP = executing.pid;
          if(cpu.execTime > 3){
            pushList(ready, executing.pid);
            executing.pid = pullList(ready);
          }
        cpu.pid = executing.pid;
        if(cpu.pid == EMPTY_ERR){
          executing.pid = pullList(ready);
          cpu.pid = executing.pid;
          if(cpu.pid == EMPTY_ERR){
            printf("No process ready to run\n");
            continue;
          }
        }
        id = pcbSerch(cpu.pid, pcb, processes);
        printf("cpuTime: %d\n", cpu.cpuTime);
        cpu = processExchange(pcb[id], cpu.execTime);
        counter = cpu.counter;
        state = execute(&cpu);

        switch(state){
          case RUNNING:
            save(cpu, pcb + id);
            pcb[id].state = state;
            break;
          case BLOCKED:
            save(cpu, pcb + id);
            pcb[id].state = state;
            pushList(blocked, executing.pid);
            executing.pid = pullList(ready);
            break;
          case WAITING:
            save(cpu, pcb + id);
            pcb[id].state = state;
            pushList(ready, executing.pid);
            executing.pid = pullList(ready);
            break;
          case TERMINATED:
            pcb[id].state = state;
            executing.pid = pullList(ready);
            break;
          case FORKING:
            printf("forking\n");
            save(cpu, pcb + id);
            addPCB(&pcb, id, ++processes, counter, Time);
            pushList(ready, processes - 1);
            break;
          case CHANGING:
          save(cpu, pcb + id);
          break;
        }
        Time++;

      break;

      case 'P':
        reporter(pcb, processes, Time);
      break;
      case 'U':
        pushList(ready, pullList(blocked));
        break;
      default:
        printf("command malformed\n");
    }
    printf("value in pcb[%d]: %d\n\n", id, pcb[id].value);
  }
  printf("finish\n");
  wait(0);
}

int getInstNum(char *string){
  return atoi(strndup(string + 2, strlen(string)));
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
  cpu.pid = -1;
  cpu.program = NULL;
  cpu.counter = 0;
  cpu.value = 0;
  cpu.execTime = 0;
  cpu.cpuTime = 0;
  return cpu;
}

CPU processExchange(PCBTable pcb, int execTime){
  CPU cpu;
  cpu.program = pcb.program;
  cpu.counter = pcb.counter;
  cpu.value = pcb.value;
  cpu.execTime = execTime + 1;
  cpu.cpuTime = pcb.cpuUsage;
  cpu.pid = pcb.pid;
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
      cpu->counter += getInstNum(instruction);
      return FORKING;
    case 'R':
      cpu->counter = -1;
      cpu->program = readProgram(strndup(instruction + 2, strlen(instruction)));
      return CHANGING;
  }
}

void save(CPU cpu, PCBTable *pcb){
  pcb->counter = cpu.counter + 1;
  pcb->value = cpu.value;
  pcb->program = cpu.program;
  pcb->cpuUsage = cpu.execTime + 1;
}

int addPCB(PCBTable **pcb, int ppcb, int size, int counter, int time){
  *pcb = (PCBTable*) realloc(*pcb, size * sizeof(PCBTable));
   (*pcb)[size - 1].pid = size - 1;
   (*pcb)[size - 1].ppid = (*pcb)[ppcb].pid;
   (*pcb)[size - 1].counter = counter + 1;
   (*pcb)[size - 1].program = (*pcb)[ppcb].program;
   (*pcb)[size - 1].value = (*pcb)[ppcb].value;
   (*pcb)[size - 1].priority = (*pcb)[ppcb].priority;
   (*pcb)[size - 1].state = WAITING;
   (*pcb)[size - 1].timeStart = time;
   (*pcb)[size - 1].cpuUsage = 0;
   return size - 1;
}

void reporter(PCBTable *pcb, int size, int Time){
        pid_t reporter;
        if((reporter = fork()) == -1){
            printf("Reporter ERROR");
            exit(FORK_ERR);
        }

        if(reporter == 0){
          int id;
          printf("********************\nEstado do sistema:\n********************\n");
          printf("TEMPO ATUAL: %i\n", Time);
          printf("PROCESSO EXECUTANDO:\n");
          for(int e = 0; e < size; e++){
            if(pcb[e].state == RUNNING){
              printf("**********\n");
              printf("PID:%i\nPPID: %i\nPRIORIDADE: %i\nVALOR: %i\nTEMPO DE INICIO: %i\nUSO DA CPU: %i\n",pcb[e].pid, pcb[e].ppid, pcb[e].priority, pcb[e].value, pcb[e].timeStart, pcb[e].cpuUsage);
              printf("**********\n");
              break;
            }
          }

          printf("BLOQUEADO:\nFila processos bloqueados:\n");

          for(int i = 0; i < size; i++){
            if(pcb[i].state == BLOCKED){
              printf("**********\n");
              printf("PID:%i\nPPID: %i\nPRIORIDADE: %i\nVALOR: %i\nTEMPO DE INICIO: %i\nUSO DA CPU: %i\n",pcb[i].pid, pcb[i].ppid, pcb[i].priority, pcb[i].value, pcb[i].timeStart, pcb[i].cpuUsage);
              printf("**********\n");
            }
          }

          printf("PROCESSOS PRONTOS:\n");
          for(int j = 0; j < size; j++){
            if(pcb[j].state == WAITING){
              printf("**********\n");
              printf("PID:%i\nPPID: %i\nPRIORIDADE: %i\nVALOR: %i\nTEMPO DE INICIO: %i\nUSO DA CPU: %i\n",pcb[j].pid, pcb[j].ppid, pcb[j].priority, pcb[j].value, pcb[j].timeStart, pcb[j].cpuUsage);
              printf("**********\n");
              }
              exit(0);
          }
        }else{
          //wait(0);
        }
}
