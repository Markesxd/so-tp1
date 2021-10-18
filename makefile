all:
	gcc commander.c -o commander
	gcc manager.c -o manager

com:
	gcc commander.c -o commander

man:
	gcc manager.c -o manager

debug:
		gcc -g commander.c -o commander
		gcc -g manager.c -o manager

debugsim:
	gcc -g p_simulado.c -o p_simulado
