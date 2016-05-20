/* 
 * print_functions.c: Virtual Machine Placement Problem - Print Functions
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */
	
/* include common header */
#include "print_functions.h"


/* print_int_matrix: prints on screen a integer matrix
 * parameter: matrix to print
 * parameter: number of individuals
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_int_matrix(int **matrix, int rows, int columns) {
	/* iterators */
	int iterator_row;
	int iterator_column;
	/* iterate on rows */	
	for (iterator_row=0; iterator_row < rows; iterator_row++) {
		printf("[DEBUG] ROW %d:\t\t",iterator_row);	
		/* iterate on columns */
		for (iterator_column = 0; iterator_column < columns; iterator_column++) {
			printf("%d ",matrix[iterator_row][iterator_column]);	
		}
		printf("\n");
	}
}

/* print_float_matrix: prints on screen a float matrix
 * parameter: matrix to print
 * parameter: number of individuals
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_float_matrix(float **matrix, int rows, int columns) {
	/* iterators */
	int iterator_row;
	int iterator_column;
	/* iterate on rows */	
	for (iterator_row=0; iterator_row < rows; iterator_row++) {
		printf("[DEBUG] ROW %d:\t",iterator_row);	
		/* iterate on columns */
		for (iterator_column = 0; iterator_column < columns; iterator_column++) {
			printf("%g\t",matrix[iterator_row][iterator_column]);	
		}
		printf("\n");
	}
}

/* print_int_array: prints on screen a int array
 * parameter: array to print
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_int_array(int *array, int columns) {
	/* iterators */
	int iterator_column;
	/* iterate on columns */
	for (iterator_column = 0; iterator_column < columns; iterator_column++) {
		printf("[DEBUG] [%d]: %d\n",iterator_column,array[iterator_column]);	
	}
}

/* print_float_array: prints on screen a float array
 * parameter: array to print
 * parameter: number of columns
 * returns: nothing, it's void
 */
void print_float_array(float *array, int columns) {
	/* iterators */
	int iterator_column;
	/* iterate on columns */
	for (iterator_column = 0; iterator_column < columns; iterator_column++) {
		printf("[DEBUG] [%d]: %f\n",iterator_column,array[iterator_column]);	
	}
}

void print_double_array(double *array, int columns) {
	/* iterators */
	int iterator_column;
	/* iterate on columns */
	for (iterator_column = 0; iterator_column < columns; iterator_column++) {
		printf("[DEBUG] [%d]: %f\n",iterator_column,array[iterator_column]);	
	}
}

/* print_float_matrix: prints on screen a int matrix
 * parameter: matrix to print
 * parameter: number of individuals
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_placement_to_file(char * file, int ** matrix, int rows, int columns) {
	
	FILE *placement_result;

	char heuristics_placement[40] = "results/";
	strcat(heuristics_placement, file);
	placement_result = fopen(heuristics_placement,"a+");

	/* iterators */
	int iterator_row;
	int iterator_column;
	/* iterate on rows */	
	for (iterator_row=0; iterator_row < rows; iterator_row++) {
		/* iterate on columns */
		for (iterator_column = 0; iterator_column < columns; iterator_column++) {
			fprintf(placement_result, "%d\t",matrix[iterator_row][iterator_column]);	
		}
		fprintf(placement_result,"\n");
	}
	fprintf(placement_result,"\n");
}

/* print_float_matrix: prints on screen a int matrix
 * parameter: matrix to print
 * parameter: number of individuals
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_utilization_matrix_to_file(char * file, float ** matrix, int rows, int columns) {
	
	FILE *utilization_result;

	char heuristics_utilization[40] = "results/";
	strcat(heuristics_utilization, file);
	utilization_result = fopen(heuristics_utilization,"a+");

	/* iterators */
	int iterator_row;
	int iterator_column;
	/* iterate on rows */	
	for (iterator_row=0; iterator_row < rows; iterator_row++) {
		/* iterate on columns */
		for (iterator_column = 0; iterator_column < columns; iterator_column++) {
			fprintf(utilization_result, "%g\t",matrix[iterator_row][iterator_column]);	
		}
		fprintf(utilization_result,"\n");
	}
	fprintf(utilization_result,"\n");
}