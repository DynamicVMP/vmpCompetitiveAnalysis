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
/* definitions */
#define H_HEADER "PHYSICAL MACHINES"
#define V_HEADER "VIRTUAL MACHINES"
#define T_HEADER "NETWORK TRAFFIC"
#define TAM_BUFFER BUFSIZ
#define CONSTANT 10000
#define BIG_COST 1000000
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
int** load_V(int v_size, char path_to_file[]);
/* load utilization of physical resources and costs of the considered objective functions */
int*** load_utilization(int **population, int **H, int **V, int number_of_individuals, int h_size, int v_size);
float** load_objectives(int **population, int ***utilization, int **H, int **V, int **T, int number_of_individuals, int h_size, int v_size, int * K, int ** network_utilization, int l_size);
