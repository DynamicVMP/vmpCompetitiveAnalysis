/*
 * common.h: Virtual Machine Placement Problem - Common Functions Header
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
#include <stdbool.h>
/* definitions */
#include "scenario.h"
#define H_HEADER "PHYSICAL MACHINES"
#define TAM_BUFFER BUFSIZ
#define CONSTANT 10


/* get the number of physical and virtual machines */
int get_h_size(char path_to_file[]);
int get_v_size(char path_to_file[]);

/* print matrix and array */
void print_int_matrix(int ** matrix, int rows, int columns);
void print_float_matrix(float ** matrix, int rows, int columns);
void print_int_array(int * array, int columns);
void print_float_array(float * array, int columns);

/* load datacenter physical resources and virtual requirements */
int** load_H(int h_size, char path_to_file[]);

/* load utilization of physical resources and costs of the considered objective functions */
int*** load_utilization(int*** utilization,int **population, int **H, float **V, int number_of_individuals, int h_size, int v_size);
float*load_weighted_sums(float **objective_functions_values_aux, float *weighted_sums, int **population,
                         int ***utilization, int **H, float **V, int number_of_individuals, int h_size, int v_size);
/* counts the quantity virtual machines requests*/
int get_v_size_per_t(float** matrix_s,int t,int max_row);
/* loads virtual machines requests*/
float** load_v_per_t(float** matrix_s,int s_size,int v_size,int t);

/*get the index of the best solution*/
int get_best_solution_index(float* weighted_sums, int number_of_individuals);

/*free memory*/
void free_float_matrix(float** matrix, int iterator_row);
void free_int_matrix(int** matrix, int iterator_row);
void free_utilization_matrix(int ***utilization,int individuals,int h_size);

/*reports in files the utilization of the datacenter physical machines and the solution obtained */
void report_solution(int *best_solution,int** utilization,float weighted_sum,int h_size,int v_size,int t);
void copy_int_matrix(int** matrix_A, int** matrix_B, int rows, int columns);
