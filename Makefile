all:
	gcc -c common.c initialization.c reparation.c local_search.c variation.c  imoavmp.c
	gcc -o imoavmp common.o initialization.o reparation.o local_search.o variation.o imoavmp.o -lm
clean:
	rm -rf *o imoavmp
init: 
	rm -rf results/*

