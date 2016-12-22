all:tchatche tchatche_serveur

tchatche:
	gcc -Wall -O2 -o tchatche cliente.c cliente_cmd.c

tchatche_serveur:
	gcc -Wall -O2 -o tchatche_serveur server.c server_cmd.c
	
clean:
	rm -f sobucli sobusrv *.o .a *~ Makefile.bak $(OUT)
