/* 
 * utils.h: Virtual Machine Placement Problem
 * Date: February, 2016
 * Author: Saúl Zalimben (szalimben93@gmail)
 */

/* include libraries */
#include "utils.h"

void free_int_matrix(int ** matrix, int rows) {

	int iterator_row;

	for (iterator_row=0; iterator_row < rows; iterator_row++) {
		free(matrix[iterator_row]);
	}		
	
}

void free_float_matrix(float ** matrix, int rows) {

	int iterator_row;

	for (iterator_row=0; iterator_row < rows; iterator_row++) {	
		free(matrix[iterator_row]);
	}
	
}