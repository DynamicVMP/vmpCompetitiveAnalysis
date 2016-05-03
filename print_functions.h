/* 
 * print_functions.h: Virtual Machine Placement Problem - Print Functions Header
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */

/* include libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

/* print matrix and array */
void print_int_matrix(int ** matrix, int rows, int columns);
void print_float_matrix(float ** matrix, int rows, int columns);
void print_int_array(int * array, int columns);
void print_float_array(float * array, int columns);
void print_double_array(double *array, int columns);

/* Print to FILE */
void print_placement_to_file(char * file, int ** matrix, int rows, int columns);
void print_utilization_matrix_to_file(char * file, float ** matrix, int rows, int columns);