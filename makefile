all:
	gcc commander.c -o commander &
	gcc p_simulado.c -o p_simulado

com:
	gcc commander.c commander

sim:
	gcc p_simulado.c -o p_simulado

debugall:
		gcc -g commander.c -o commander &
		gcc -g p_simulado.c -o p_simulado

debugsim:
	gcc -g p_simulado.c -o p_simulado
