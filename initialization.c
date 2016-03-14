/* 
 * initialization.c: Virtual Machine Placement Problem - Initialization Stage
 * Date: February, 2016
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */

/* include initialization stage header */
#include "initialization.h"

/* utilization_initialization: initial utilization matrix 
 * parameter: number of physical machines
 * parameter: number of resources
 * return: initial utilization matrix
 */
float** utilization_initialization(int h_size, int resources) {
	
	float **utilization = (float **) malloc (h_size *sizeof (float *));
	/* iterators */
	int iterator_physical;
	int iterator_physical_position;
	/* iterate on individuals */
	
	for (iterator_physical=0; iterator_physical < h_size; iterator_physical++) {	
		utilization[iterator_physical] = (float *) malloc (resources *sizeof (float));
		/* iterate on positions of an individual */
		for (iterator_physical_position = 0; iterator_physical_position < resources; iterator_physical_position++) {
			utilization[iterator_physical][iterator_physical_position] = 0;
		}
	}

	return utilization;
}

/* placement_initialization: initial placement matrix 
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * return: initial placement matrix
 */
int** placement_initialization(int vm_parameter, int v_size) {
	
	int **placement = (int **) malloc (vm_parameter *sizeof (int *));
	/* iterators */
	int iterator_physical;
	int iterator_physical_position;
	
	for (iterator_physical=0; iterator_physical < vm_parameter; iterator_physical++) {	
		placement[iterator_physical] = (int *) malloc (v_size *sizeof (int));
		/* iterate on positions of an individual */
		for (iterator_physical_position = 0; iterator_physical_position < v_size; iterator_physical_position++) {
			placement[iterator_physical][iterator_physical_position] = 0;
		}
	}
	return placement;
}
