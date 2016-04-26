/*
 * variation.h: Virtual Machine Placement Problem - Genetic Operators Functions Header
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "common.h"
#define TOURNAMENT_SIZE 2

/* function headers definitions */
int selection(float *objective_function, int number_of_individuals);
int **selection_and_crossover(int **populationQ, int **populationP, float *weighted_sums_P, int number_of_individuals,
                              int v_size);
int ** mutation(int **population, float **V, int number_of_individuals, int h_size, int v_size, int max_SLA);
int ** population_evolution(int **P, int **Q, float * weighted_sums_P, float *weighted_sums_Q, int number_of_individuals, int v_size);
int ** crossover_individuals(int **populationQ,int **populationP, int position_parent1, int position_parent2, int v_size);
void quicksort_sort(float *weighted_sums,int ** population, int first_index, int last_index);
int quicksort_partition(float *weighted_sums,int ** population, int first_index, int last_index);
