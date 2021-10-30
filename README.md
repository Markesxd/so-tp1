# Primeiro Trabalho Pratico de Sistemas Operacionais

## O objetivo deste trabalho é criar um programa que simula algumas das funções básicas de um sitema operacional.

### Conteudo

- Commander  
Processo que inicia o sistema e envia comandos para o manager.

- Manager
O manager gerencia os processos simulados criados durante a execução do programa, ele escalona os processos permitindo que eles ocupem a CPU 5 vezes antes de ser interrompidos.

- Processo simulado  
Guarda um valor e pode fazer somas e subtrações nele (ou também pode altera-lo por completo); também pode criar um processo simulado filho que realizará o fim da sua cadeia de comandos. É possivel substituir o programa executoda pelo processo.


## Compilação e execução

Para compilar:
make all

Para executar:
./commander
