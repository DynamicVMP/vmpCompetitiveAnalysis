/* 
 * heuristics.c: Virtual Machine Placement Problem - Heuristics Functions Header
 * Date: 21-02-2016
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 * Author: Rodrigo Ferreira (rodrigofepy@gmail.com)
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

	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
		// If request is in time 0, we directle allocate VM

		if (check_resources(request, utilization[iterator_physical], H[iterator_physical])) {
						
			utilization[iterator_physical][0] += request[4]*request[7]/100;
			utilization[iterator_physical][1] += request[5]*request[8]/100;

			// Allocate la VM to VM
			allocate_VM_to_PM(placement, request, iterator_physical);
			// printf("Placement: %d\n", placement[iterator_physical][ (int) request[1]][ (int) request[2]][ (int) request[3]] );

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
 * Allocate VM to PM
 * Update the placement matrix
 */
void allocate_VM_to_PM(int ****placement, float *request, int pm ) {

	printf("\nLos valores son: %d%d%d%d\n", pm, (int) request[1], (int) request[2], (int) request[3] );
	placement[pm][(int) request[1]] [ (int) request[2]] [ (int) request[3]] = 1;

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
//		if (check_resources(request, utilization[PM_ordered_list->h_index], H[PM_ordered_list->h_index])) {
//			printf("Allocate VM into PM %d",PM_ordered_list->h_index);
//			//allocateVM(H[PM_ordered_list->h_index]);
//			return 1;
//		}
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

/**
 * Check if the PM have enough resources for VM
 */
bool check_resources(float *request, float *utilization, int *H) {

	return (utilization[0] + (request[4]*request[7]/100) <= H[0] 
			&& utilization[1] + (request[5]*request[8]/100) <= H[1]);
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