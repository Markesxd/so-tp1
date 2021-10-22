#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

stateList* initializeList(){
  stateList *node;
  node = (stateList*) malloc(sizeof(stateList));
  node->next = NULL;
  node->pid = -1;
  return node;
}

void pushList(stateList *node, int id){
  while(node->next != NULL) node = node->next;
  node->next = initializeList();
  node->next->pid = id;
}

int pullList(stateList *node){
  stateList *aux;
  int index = 0;
  aux = node->next->next;
  index = node->next->pid;
  free(node->next);
  node->next = aux;
  return index;
}

void printList(stateList *node){
  while(node->next != NULL){
    node = node->next;
    printf("%d ", node->pid);
  }
  printf("\n");
}
