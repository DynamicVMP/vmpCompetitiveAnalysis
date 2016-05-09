all:
	gcc -c common.c initialization.c reparation.c local_search.c variation.c  imoavmp.c
	gcc -o imoavmp common.o initialization.o reparation.o local_search.o variation.o imoavmp.o -lm

clean:
	rm -rf *o imoavmp

init:
	rm -rf results/*

run_all_scenarios: 
	ls inputs  | while read x; do ./imoavmp inputs/$$x; done

run_normal_1:
	./imoavmp inputs/100.1-Normal

run_normal_2:
	./imoavmp inputs/100.2-Normal

run_normal_3:
	./imoavmp inputs/100.3-Normal

run_normal_4:
	./imoavmp inputs/100.4-Normal

run_normal_5:
	./imoavmp inputs/100.5-Normal

run_poisson_1:
	./imoavmp inputs/100.1-Poisson

run_poisson_2:
	./imoavmp inputs/100.2-Poisson

run_poisson_3:
	./imoavmp inputs/100.3-Poisson

run_poisson_4:
	./imoavmp inputs/100.4-Poisson

run_poisson_5:
	./imoavmp inputs/100.5-Poisson

run_uniforme_1:
	./imoavmp inputs/100.1-Uniforme

run_uniforme_2:
	./imoavmp inputs/100.2-Uniforme

run_uniforme_3:
	./imoavmp inputs/100.3-Uniforme

run_uniforme_4:
	./imoavmp inputs/100.4-Uniforme

run_uniforme_5:
	./imoavmp inputs/100.5-Uniforme
