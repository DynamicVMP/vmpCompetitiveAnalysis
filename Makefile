all:
	gcc -c common.c print_functions.c utils.c scenario.c heuristics.c initialization.c imavmp.c
	gcc -o imavmp common.o initialization.o print_functions.o utils.o scenario.o heuristics.o imavmp.o -lm

clean:
	rm -rf *o imavmp

init: 
	rm -rf results/*
	rm -rf *o imavmp

run_mini_input:
	./imavmp inputs/mini_input.vmp

run_all_inputs_with_all_heuristics:
	ls inputs $1 | while read x; do for y in 1 2 3 4 5; do ./imavmp inputs/$$x $$y; done; done

run_normal_1:
	./imavmp inputs/100.1-Normal

run_normal_2:
	./imavmp inputs/100.2-Normal

run_normal_3:
	./imavmp inputs/100.3-Normal

run_normal_4:
	./imavmp inputs/100.4-Normal

run_normal_5:
	./imavmp inputs/100.5-Normal

run_poisson_1:
	./imavmp inputs/100.1-Poisson

run_poisson_2:
	./imavmp inputs/100.2-Poisson

run_poisson_3:
	./imavmp inputs/100.3-Poisson

run_poisson_4:
	./imavmp inputs/100.4-Poisson

run_poisson_5:
	./imavmp inputs/100.5-Poisson

run_uniform_1:
	./imavmp inputs/100.1-Uniforme

run_uniform_2:
	./imavmp inputs/100.2-Uniforme

run_uniform_3:
	./imavmp inputs/100.3-Uniforme

run_uniform_4:
	./imavmp inputs/100.4-Uniforme

run_uniform_5:
	./imavmp inputs/100.5-Uniforme

