typedef struct a fila;

struct a{
  int indice;
  fila *prox;
};

fila* inicializaFila(){
  fila *vertice;
  vertice = (fila*) malloc(sizeof(fila));
  vertice->prox = NULL;
  vertice->indice = -1;
  return vertice;
}

void enfileiraFila(fila *vertice, int indice){
  while(vertice->prox != NULL) vertice = vertice->prox;
  vertice->prox = inicializaFila();
  vertice->prox->indice = indice;
}

int desenfileiraFila(fila *vertice){
  fila *aux;
  int indice = 0;
  aux = vertice->prox->prox;
  indice = vertice->prox->indice;
  free(vertice->prox);
  vertice->prox = aux;
  return indice;
}

void imprimeFila(fila *vertice){
  while(vertice->prox != NULL){
    vertice = vertice->prox;
    printf("%d ", vertice->indice);
  }
  printf("\n");
}
