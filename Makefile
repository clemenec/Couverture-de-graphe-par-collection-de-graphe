#Définition des macros
CC = gcc
CFLAGS = -Wall -c -g
LDFLAGS = -lm
CSRC = *.c
OBJS = opti_arbre.o\
	structures.o lecture_graph.o parcours.o

.PHONY: run
run: opti_arbre
	python3 drawingraph.py
	echo $(CSRC)
	./opti_arbre

debug: opti_arbre
	valgrind --leak-check=full ./opti_arbre

opti_arbre: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

opti_arbre.o: opti_arbre.c *.h
	$(CC) $(CFLAGS) $<

%.o: %.c %.h
	$(CC) $(CFLAGS) $<

.PHONY: clean
clean: 
	rm -f *.o
	rm -f opti_arbre
	ls -l