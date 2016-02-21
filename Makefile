all:
	gcc -c common.c initialization.c reparation.c local_search.c variation.c network.c pareto.c imavmp.c
	gcc -o imavmp common.o initialization.o reparation.o local_search.o variation.o network.o pareto.o imavmp.o -lm
clean:
	rm -rf *o imavmp
init: 
	rm -rf results/pareto_*
debug:
	gcc -g -c common.c initialization.c reparation.c local_search.c variation.c network.c pareto.c imavmp.c
	gcc -g -o imavmp common.o initialization.o reparation.o local_search.o variation.o network.o pareto.o imavmp.o -lm
