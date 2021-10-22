typedef struct a stateList;

struct a{
  int pid;
  stateList *next;
};

stateList* initializeList();

void pushList(stateList *node, int id);

int pullList(stateList *node);

void printList(stateList *node);
