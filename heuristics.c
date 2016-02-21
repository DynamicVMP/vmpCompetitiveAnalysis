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
 
struct virtual_machine {
	int t;
	int servicio;
	int datacenter;
	int vm_id;
	int cpu;
	int ram;
	int net;
	float revenue;
	int SLA;
};

/* 
 * Firt Fit Algorithm
 */
int first_fit(float *request, float **utilization, int **placement, int **H, int h_size) {

	int iterator_physical; 
	int iterator_scenario;
	int requestRejected = 0;

	for (iterator_physical = 0; iterator_physical < h_size; ++iterator_physical) {
		// If request is in time 0, we directle allocate VM
		if (checkResources(request, utilization[iterator_physical], H[iterator_physical], h_size)) {
			printf("\naqui debemos verficar los recursos de todas las PM");
			//allocateVM(H[iterator_physical]);
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


bool checkResources(float *request, float *utilization, int *H, int h_size) {

	/*printf("Utilization:\n");
	print_float_array(utilization, 2);

	printf("H:\n");
	print_int_array(H, h_size);*/

	printf("\nCheck CPU resources\n");
	if (utilization[0] + (request[4]*request[7]/100) <= H[0] 
		&& utilization[1] + (request[5]*request[8]/100) <= H[1]) {

		utilization[0] =+ request[4]*request[7]/100;
		utilization[1] =+ request[5]*request[8]/100;

		// magic_functions_allocate_VM_to_PM
		/*printf("Request RAM: %g\n", request[4]);
		printf("Real Use RAM: %g%c\n", request[7], '%');
		printf("Utilizacion real: %g\n", (request[4]*request[7]/100) );
		
		printf("Existe lugar acorde CPU\n");*/

		printf("Utilizacion CPU H: %g\n", utilization[0]);
		printf("Utilizacion RAM H: %g\n", utilization[1]);

		return true;
	} else {
		return false;
		printf("No existe lugar");
	}

/*	printf("\nCheck RAM resources\n");
	if (utilization[1] + (request[5]*request[8]/100) <= H[1] ) {
		printf("Request RAM: %g\n", request[5]);
		printf("Real Use RAM: %g%c\n", request[8], '%');
		printf("Utilizacion RAM H: %g\n", utilization[1] );
		printf("Utilizacion real: %g\n", (request[5]*request[8]/100) );
		printf("Existe lugar acorde RAM\n");
	} else {
		return false;
		printf("No existe lugar");
	}*/
}