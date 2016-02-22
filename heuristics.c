/* 
 * heuristics.c: Virtual Machine Placement Problem - Heuristics Functions Header
 * Date: 21-02-2016
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */

/* include libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "print_functions.h"
#include "scenario.h"
#include "heuristics.h"

/* 
 * First Fit Algorithm
 */
int first_fit(float *request, float **utilization, int ****placement, int **H, int h_size) {

	int iterator_physical; 
	int iterator_scenario;
	int requestRejected = 0;

	for (iterator_physical = 0; iterator_physical < h_size; ++iterator_physical) {
		// If request is in time 0, we directle allocate VM
		if (check_resources(request, utilization[iterator_physical], H[iterator_physical], h_size)) {
			print_float_array(utilization[iterator_physical], 2);
			
			utilization[iterator_physical][0] =+  request[4]*request[7]/100;
			utilization[iterator_physical][1] =+ request[5]*request[8]/100;

			// Allocate la VM to VM
			allocate_VM_to_PM(placement, request, iterator_physical);

			printf("%g\n", request[0]);
			return 1;
		}
	}
	/*if (SLA == MAX_SLA) {
		int allocate = forceAllocate(S);
		if (allocate != 0) {
			requestRejected++;
		}
	} else {
		requestRejected++;
	}*/
	printf("Request Rejected: %d\n", requestRejected);
}

/**
 * Check if the PM have enough resources for VM
 */
bool check_resources(float *request, float *utilization, int *H, int h_size) {

	printf("\nCheck CPU resources\n");
	if (utilization[0] + (request[4]*request[7]/100) <= H[0] 
		&& utilization[1] + (request[5]*request[8]/100) <= H[1]) {

		printf("Actual CPU H: %g\n", utilization[0]);
		printf("Actual RAM H: %g\n", utilization[1]);

		return true;
	} else {
		return false;
		printf("No existe lugar");
	}
}
/**
 *
 *
 */
void allocate_VM_to_PM(int ****placement, float *request, int pm ) {

	placement[pm] = (int ***) malloc (sizeof (int ***));
	placement[pm][ (int) request[1]] = (int **) malloc (sizeof (int **));
	placement[pm][ (int) request[1]][ (int) request[2]] = (int *) malloc (sizeof (int *));
	placement[pm][ (int) request[1]][ (int) request[2]][ (int) request[3]] = 1;
}
