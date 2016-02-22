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

typedef struct {
	int h_index;
	float weight;
	PM_weight_pair_node* next;
} PM_weight_pair_node;

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

int best_fit(float *request, float **utilization, int **placement, int **H, int h_size) {
	return best_or_worst_fit(true, request, utilization, placement, H, h_size);
}

int worst_fit(float *request, float **utilization, int **placement, int **H, int h_size) {
	return best_or_worst_fit(false, request, utilization, placement, H, h_size);
}

int best_or_worst_fit(bool is_best,float *request, float **utilization, int **placement, int **H, int h_size) {

	int iterator_physical;
	int iterator_scenario;
	int request_rejected = 0;
	PM_weight_pair_node* PM_ordered_list = malloc(sizeof(PM_weight_pair_node));
	float weight_PM = 0.0;

	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
		weight_PM = calculate_weight(placement, H[iterator_physical], iterator_physical);
		insert_PM_to_ordered_list(is_best, &PM_ordered_list, weight_PM, iterator_physical);
	}

	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++){
		if (checkResources(request, utilization[PM_ordered_list->h_index], H[PM_ordered_list->h_index], h_size)) {
			printf("Allocate VM into PM %d",PM_ordered_list->h_index);
			//allocateVM(H[PM_ordered_list->h_index]);
			return 1;
		}
		PM_ordered_list = PM_ordered_list->next;
	}

//	if (SLA == MAX_SLA) {
//		int allocate = forceAllocate(S);
//		if (allocate != 0) {
//			requestRejected++;
//		}
//	} else {
//		requestRejected++;
//	}

	return 0;
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

float calculate_weight(int **placement, int *H, int h_index){
	float weight_PM = 0.0;
	return weight_PM;
}

void insert_PM_to_ordered_list(bool is_best, PM_weight_pair_node** PM_ordered_list, float weight_PM, int h_index){

	bool ( *comparator )( float, float );
	PM_weight_pair_node* new_node = malloc(sizeof(PM_weight_pair_node));

	new_node->weight = weight_PM;
	new_node->h_index = h_index;
	new_node->next = NULL;

	if(is_best){
		comparator = &best_comparator;
	}else{
		comparator = &worst_comparator;
	}

	if((*comparator)(new_node->weight,(*PM_ordered_list)->weight)){
		new_node->next = *PM_ordered_list;
		*PM_ordered_list = new_node;
		return;
	}

	PM_weight_pair_node* parent = *PM_ordered_list;
	PM_weight_pair_node* actual = (*PM_ordered_list)->next;

	while(actual != NULL){
		if((*comparator)(new_node->weight,actual->weight)){
			new_node->next = parent->next;
			parent->next = new_node;
			return;
		}
		parent = actual;
		actual = actual->next;
	}
	parent->next = new_node;
	return;

}

bool best_comparator(float weight_A, float weight_B){
	return weight_A < weight_B;
}

bool worst_comparator(float weight_A, float weight_B){
	return weight_A > weight_B;
}