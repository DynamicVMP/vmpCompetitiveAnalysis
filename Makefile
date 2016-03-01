all:
	gcc -c common.c print_functions.c utils.c scenario.c heuristics.c initialization.c imavmp.c
	gcc -o imavmp common.o initialization.o print_functions.o utils.o scenario.o heuristics.o imavmp.o -lm

clean:
	rm -rf *o imavmp
init: 
	rm -rf results/*

run_mini_input:
	./imavmp inputs/mini_input.vmp

run:
	./imavmp inputs/4x8.vmp
