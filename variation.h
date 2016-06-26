/*
 * variation.h: Virtual Machine Placement Problem - Genetic Operators Functions Header
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include libraries */

#define TOURNAMENT_SIZE 2
#define NOT_DERIVED 0
#include <stdbool.h>

/* function headers definitions */

int selection(double *objective_function, int number_of_individuals);
int **selection_and_crossover(int **populationQ, int **populationP, double *weighted_sums_P, int number_of_individuals,
                              int v_size);
int ** mutation(int **population, float **V, int number_of_individuals, int h_size, int v_size, int max_SLA);
int ** population_evolution(int **P, int **Q, double * weighted_sums_P, double *weighted_sums_Q, int number_of_individuals, int v_size);
int ** crossover_individuals(int **populationQ,int **populationP, int position_parent1, int position_parent2, int v_size);
void quicksort_sort(double *weighted_sums,int ** population, int first_index, int last_index);
int quicksort_partition(double *weighted_sums,int ** population, int first_index, int last_index);
void free_int_matrix(int** matrix, int iterator_row);