all:
	gcc commander.c -o commander &
	gcc p_simulado.c -o p_simulado &
	gcc manager.c -o manager

com:
	gcc commander.c commander

sim:
	gcc p_simulado.c -o p_simulado

man:
	gcc manager.c -o manager

debugall:
		gcc -g commander.c -o commander &
		gcc -g p_simulado.c -o p_simulado
		gcc -g manager.c -o manager

debugsim:
	gcc -g p_simulado.c -o p_simulado
