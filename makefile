all:
	gcc commander.c -o commander
	gcc manager.c fila.c -o manager

com:
	gcc commander.c -o commander

man:
	gcc manager.c fila.c -o manager

debug:
		gcc -g commander.c -o commander
		gcc -g manager.c fila.c -o manager

sanitize:
	gcc -g3 -fsanitize=address commander.c -o commander	
	gcc -g3 -fsanitize=address manager.c fila.c -o manager
