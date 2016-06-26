/*
 * initialization.h: Virtual Machine Placement Problem - Initialization Stage Header
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define OBJECTIVE_FUNCTIONS 4
#define NOT_DERIVED 0
#define RESOURCES 3

/* function headers definitions */
int** initialization(int** population,int number_of_individuals, int h_size, int v_size,int previous_v_size, float **V, int option);
int generate_solution_position(int max_possible, int SLA);
void create_structures(int*** population_P, int*** population_Q, int**** utilization_P, int**** utilization_Q, double** weighted_sums_P,
                       double** weighted_sums_Q, double***objectives_functions_values, float*** wasted_resources_ojb, int number_of_individuals, int v_size, int h_size);
