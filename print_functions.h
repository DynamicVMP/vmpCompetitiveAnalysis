/* 
 * print_functions.h: Virtual Machine Placement Problem - Print Functions Header
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Edited by: Saúl Zalimben 20-02-2016
 */

/* include libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* print matrix and array */
void print_int_matrix(int ** matrix, int rows, int columns);
void print_float_matrix(float ** matrix, int rows, int columns);
void print_int_array(int * array, int columns);
void print_float_array(float * array, int columns);