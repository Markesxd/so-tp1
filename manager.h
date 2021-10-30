#define MAX_INSTRUCTION_LEN 25
#define RUNNING 0
#define BLOCKED 1
#define WAITING 2
#define TERMINATED 3
#define FORKING 4
#define CHANGING 5

typedef struct {
  int pid;
  char** program;
  int counter;
  int value;
  int execTime;
  int cpuTime;
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

int getInstNum(char *string);

char** readProgram(char *path);

PCBTable* pcbInit();

int pcbSerch(int pid, PCBTable *pcb, int size);

CPU cpuInit();

CPU processExchange(PCBTable pcb, int cpuTime);

int execute(CPU *cpu);

void save(CPU cpu, PCBTable *pcb);

int addPCB(PCBTable **pcb, int ppcb, int size, int counter, int time);

void reporter(PCBTable *pcb, int size, int Time);
