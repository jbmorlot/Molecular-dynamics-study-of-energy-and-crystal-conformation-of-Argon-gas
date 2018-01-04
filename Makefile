CC=g++
CFLAGS=-W -Wall -ansi -pedantic 
LDFLAGS=
EXEC=dynamique_mol
EXEC2=remote

all: $(EXEC) $(EXEC2)

dynamique_mol:	vecteur.o	toolbox.o	particule.o	defines.o	world.o	main.o		
	$(CC)  -o $@ $^ $(LDFLAGS)

remote: Remote_control.o
	$(CC)  -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm *.o	rm Variables_simulation.txt	rm remote	rm dynamique_mol	rm -rf Simulation2_Etude_reseaux_cristallins_argon Simulation1_Etude_systeme_Energie_constante

mrproper: clean
	rm $(EXEC) $(EXEC2)


