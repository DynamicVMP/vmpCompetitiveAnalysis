/* 
 * initialization.c: Virtual Machine Placement Problem - Initialization Stage
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include initialization stage header */
#include "initialization.h"

/* utilization: initial utilization matrix */
float** utilization_initialization(int h_size, int resources) {
	
	float **utilization = (float **) malloc (resources *sizeof (float *));
	/* iterators */
	int iterator_physical;
	int iterator_physical_position;
	/* iterate on individuals */
	
	for (iterator_physical=0; iterator_physical < resources; iterator_physical++) {	
		utilization[iterator_physical] = (float *) malloc (h_size *sizeof (float));
		/* iterate on positions of an individual */
		for (iterator_physical_position = 0; iterator_physical_position < h_size; iterator_physical_position++) {
			utilization[iterator_physical][iterator_physical_position] = 0;
		}
	}

	return utilization;
}

/* utilization: initial utilization matrix */
int** placement_initialization(int h_size, int v_size) {
	
	int **placement = (int **) malloc (v_size *sizeof (int *));
	/* iterators */
	int iterator_physical;
	int iterator_physical_position;
	
	for (iterator_physical=0; iterator_physical < v_size; iterator_physical++) {	
		placement[iterator_physical] = (int *) malloc (h_size *sizeof (int));
		/* iterate on positions of an individual */
		for (iterator_physical_position = 0; iterator_physical_position < h_size; iterator_physical_position++) {
			placement[iterator_physical][iterator_physical_position] = 0;
		}
	}
	return placement;
}
