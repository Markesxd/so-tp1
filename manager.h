#define MAX_INSTRUCTION_LEN 25
#define RUNNING 0
#define BLOCKED 1
#define WAITING 2
#define TERMINATED 3

typedef struct {
  char** program;
  int counter;
  int value;
  int execTime;
} CPU;

typedef struct {
  int pid;
  int ppid;
  int counter;
  char** program;
  int value;
  int priority;
  int state;
  int timeStart;
  int cpuUsage;
} PCBTable;

char* slice(char *string, int start, int end);

int getInstNum(char *string);

char** readProgram(char *path);

PCBTable* pcbInit();

int pcbSerch(int pid, PCBTable *pcb, int size);

CPU cpuInit();

CPU processExchange(PCBTable pcb);

int execute(CPU *cpu);

PCBTable save(CPU cpu);
