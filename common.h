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
#define H_HEADER "PHYSICAL MACHINES"
#define S_HEADER "SCENARIO"
#define TAM_BUFFER BUFSIZ
#define CONSTANT 1000
#define SIGMA_POWER 1.279626366
#define SIGMA_REVENUE 1.559828872
#define SIGMA_RESOURCES 2.370420922
#define SIGMA_QOS 1.930521098
#define  VM_FEATURES 10
#define RESOURCES 3
#define OBJECTIVE_FUNCTIONS 4

/* get the number of physical and virtual machines */
int get_h_size(char path_to_file[]);

/* function headers definitions */
int get_s_size(char path_to_file[]);
float** load_S(int s_size, char path_to_file[]);

/* print matrix and array */
void print_int_matrix(int ** matrix, int rows, int columns);
void print_float_matrix(float ** matrix, int rows, int columns);
void print_double_matrix(double ** matrix, int rows, int columns);
void print_int_array(int * array, int columns);
void print_float_array(float * array, int columns);
void print_double_array(double * array, int columns);

/* load datacenter physical resources and virtual requirements */
int** load_H(int h_size, char path_to_file[]);

/* load utilization of physical resources and costs of the considered objective functions */
int*** load_utilization(int*** utilization,int **population, int **H, float **V, int number_of_individuals, int h_size, int v_size);
double*load_weighted_sums(double **objective_functions_values, double *weighted_sums, int **population, int ***utilization, int **H, float **V, int number_of_individuals, int h_size, int v_size, double qos_a_priori_t, float revenue_a_priori_t, int *OF_calc_count);
double** load_objective_functions(double **objective_functions_values, int **population, int ***utilization, int **H, float **V, int number_of_individuals, int h_size, int v_size, int* OF_calc_count);
double calculates_weighted_sum(float power_normalized, float delta_revenue_normalized, float wasted_resources_normalized, double delta_qos_normalized);
/* counts the quantity virtual machines requests*/
int get_v_size_per_t(float** matrix_s,int t,int max_row);
/* loads virtual machines requests*/
float** load_v_per_t(float** matrix_s, int s_size, int v_size, double *qos_a_priori_t, float *revenue_a_priori_t, int t);

/*get the index of the best solution*/
int get_best_solution_index(double* weighted_sums, int number_of_individuals);
int get_worst_solution_index(double* weighted_sums, int number_of_individuals);

/*free memory*/
void free_float_matrix(float** matrix, int iterator_row);
void free_double_matrix(double** matrix, int iterator_row);
void free_int_matrix(int** matrix, int iterator_row);
void free_utilization_matrix(int ***utilization,int individuals,int h_size);

/*reports in files the utilization of the datacenter physical machines and the solution obtained */
void report_solution(int *best_solution,double* objective_functions_solution,int** utilization,double weighted_sum,float** V,int h_size,int v_size,char* file_postfix,int t);
void copy_int_matrix(int** matrix_A, int** matrix_B, int rows, int columns);
void copy_int_array(int* array_A, int* array_B, int array_size);