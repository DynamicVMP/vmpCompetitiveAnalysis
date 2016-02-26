/* 
 * heuristics.c: Virtual Machine Placement Problem - Heuristics Functions Header
 * Date: 21-02-2016
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 * Author: Rodrigo Ferreira (rodrigofepy@gmail.com)
 */

/* include libraries */
#include "heuristics.h"

/**
 * Allocate VM to PM
 * Update the placement matrix
 */
bool allocate_VM_to_PM(int **placement, float *request, int pm ) {

	placement[(int) request[3]][pm] = 1;
	return true;
}

/**
 * Check if the PM have enough resources for VM
 */
bool check_resources(float *request, float *utilization, int *H) {

	return (utilization[0] + (request[4]*request[7]/100) <= H[0] 
			&& utilization[1] + (request[5]*request[8]/100) <= H[1]
			&& utilization[2] + (request[6]*request[9]/100) <= H[2]);
}


/*
 * First Fit Algorithm
 */
int first_fit(float *request, float **utilization, int **placement, int **H, int h_size) {

	int iterator_physical; 
	int iterator_scenario;

	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
		// If request is in time 0, we directle allocate VM

		if (check_resources(request, utilization[iterator_physical], H[iterator_physical])) {		
			// Allocate la VM to VM		
			if(allocate_VM_to_PM(placement, request, iterator_physical)) {
				utilization[0][iterator_physical] += request[4]*request[7]/100;
				utilization[1][iterator_physical] += request[5]*request[8]/100;
				utilization[2][iterator_physical] += request[6]*request[9]/100;	

				// Save FILE
				print_placement_to_file("first_fit/placement_result", placement, 4, h_size);
				print_utilization_to_file("first_fit/cpu_utilization", utilization[0], h_size);
				print_utilization_to_file("first_fit/ram_utilization", utilization[1], h_size);
				print_utilization_to_file("first_fit/net_utilization", utilization[2], h_size);

			}
			
			return 1;
		}
	}
	return 0;
}


int best_or_worst_fit(bool is_best,float *request, float **utilization, int **placement, int **H, int h_size) {

	int iterator_physical;
	int iterator_scenario;
	int request_rejected = 0;
	PM_weight_pair_node* PM_ordered_list = malloc(sizeof(PM_weight_pair_node));
	PM_weight_pair_node* clean_list;
	float weight_PM = 0.0;
	
	// printf("Creating ordered list of PM based on average weight\n");
	
	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
		weight_PM = calculate_weight(utilization, H[iterator_physical], iterator_physical);
		// printf("Weight calculated for iterator %d\n", iterator_physical);
		insert_PM_to_ordered_list(is_best, &PM_ordered_list, weight_PM, iterator_physical);
		// printf("PM %d inserted on ordered list\n",iterator_physical);
	}

	// printf("Ordered list created\n");
	clean_list = PM_ordered_list;
	
	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++){
		// // printf("Checking resources for index %d\n",PM_ordered_list->h_index);
		if (check_resources(request, utilization[PM_ordered_list->h_index], H[PM_ordered_list->h_index])) {
			// Allocate the VM into the PM
			// printf("Allocating VM\n");
			if(allocate_VM_to_PM(placement, request, PM_ordered_list->h_index)) {

				utilization[0][PM_ordered_list->h_index] += request[4]*request[7]/100;
				utilization[1][PM_ordered_list->h_index] += request[5]*request[8]/100;
				utilization[2][PM_ordered_list->h_index] += request[6]*request[9]/100;
				
				// Save FILE
				if(is_best) {
					print_placement_to_file("best_fit/placement_result", placement, 4, h_size);
					print_utilization_to_file("best_fit/cpu_utilization", utilization[0], h_size);
					print_utilization_to_file("best_fit/ram_utilization", utilization[1], h_size);
					print_utilization_to_file("best_fit/net_utilization", utilization[2], h_size);	
				} else {
					print_placement_to_file("worst_fit/placement_result", placement, 4, h_size);
					print_utilization_to_file("worst_fit/cpu_utilization", utilization[0], h_size);
					print_utilization_to_file("worst_fit/ram_utilization", utilization[1], h_size);
					print_utilization_to_file("worst_fit/net_utilization", utilization[2], h_size);	
				}
				

			}
			// printf("VM allocated %d\n", clean_list->h_index);
			// free_list(clean_list);
			return 1;
		}
		// printf("Iterating over the ordered list\n");
		PM_ordered_list = PM_ordered_list->next;
	}
	// printf("VM not allocated\n");
	// free_list(clean_list);
	return 0;
}

int best_fit(float *request, float **utilization, int **placement, int **H, int h_size) {
	return best_or_worst_fit(true, request, utilization, placement, H, h_size);
}

int worst_fit(float *request, float **utilization, int **placement, int **H, int h_size) {
	return best_or_worst_fit(false, request, utilization, placement, H, h_size);
}


float calculate_weight(float **utilization, int *H, int h_index){
	float weight_PM = 0.0;
	weight_PM += utilization[0][h_index] / H[0];
	weight_PM += utilization[1][h_index] / H[1];
	weight_PM += utilization[2][h_index] / H[2];
	return weight_PM;
}

void insert_PM_to_ordered_list(bool is_best, PM_weight_pair_node** PM_ordered_list, float weight_PM, int h_index) {

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

	if((*comparator)(new_node->weight,(*PM_ordered_list)->weight)) {
		// printf("Insert first in list\n");
		new_node->next = *PM_ordered_list;
		*PM_ordered_list = new_node;
		// printf("Index %d\n",new_node->h_index);
		// printf("Index %d\n",(*PM_ordered_list)->h_index);
		return;
	}

	PM_weight_pair_node* parent = *PM_ordered_list;
	PM_weight_pair_node* actual = parent->next;
	
	while(actual != NULL) {
		// // printf("IN WHILE  Index %d\n", actual->h_index);
		if((*comparator)(new_node->weight,actual->weight)){
			// printf("new_node %d\n", new_node->h_index);
			new_node->next = actual;
			parent->next = new_node;
			// printf("new_node %d\n", new_node->h_index);
			// printf("Insert in list\n");
			// printf("Index %d\n", actual->h_index);
			return;
		}
		parent = actual;
		actual = actual->next;
	}
	parent->next = new_node;
	// printf("Insert last in list\n");
	// printf("Index %d\n",new_node->h_index);
	return;

}

bool best_comparator(float weight_A, float weight_B){
	return weight_A < weight_B;
}

bool worst_comparator(float weight_A, float weight_B){
	return weight_A > weight_B;
}

void free_list(PM_weight_pair_node* list_to_free){
	PM_weight_pair_node* tmp_pointer;
	// printf("Release memory\n");
	while(list_to_free != NULL){
		tmp_pointer = list_to_free;
		list_to_free = list_to_free->next;
		// printf("free begin\n");
		free(tmp_pointer);
		// printf("free end\n");
	}
	// printf("Memory released\n");
}