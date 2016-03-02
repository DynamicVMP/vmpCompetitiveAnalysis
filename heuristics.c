/* 
 * heuristics.c: Virtual Machine Placement Problem - Heuristics Functions Header
 * Date: February, 2016
 * Author: Rodrigo Ferreira (rodrigofepy@gmail.com) 
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */

/* include libraries */
#include "heuristics.h"

/**
 * allocate_VM_to_PM: update the placement matrix, allocate VM to PM
 * parameter: placement matrix
 * parameter: request of VM
 * parameter: physical machine
 * return: True, if the VM war correctly allocate
 * 		   False, other case.	
 */
bool allocate_VM_to_PM(int **placement, float *request, int pm ) {

	placement[(int) request[3]][pm] = 1;
	return true;
}

/**
 * check_resources: check the available resources of the physical machine
 * parameter: request of VM
 * parameter: array utilization
 */
bool check_resources(float *request, float *utilization, int *H) {

	return (utilization[0] + (request[4]*request[7]/100) <= H[0] 
			&& utilization[1] + (request[5]*request[8]/100) <= H[1]
			&& utilization[2] + (request[6]*request[9]/100) <= H[2]);
}



// ################################## Heuristics ############################################### //
/*
 * first_fit: First Fit Algorithm
 * parameter: request of VM
 * parameter: utilization matrix	
 * parameter: placement matrix
 * parameter: number of physical machines
 * returns: True (1), if the VM was correctly allocate.
 * 			False(0), other case.
 */
int first_fit(float *request, float **utilization, int **placement, int **H, int h_size, int *request_rejected) {

	int iterator_physical; 
	int iterator_scenario;

	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
		// If request is in time 0, we directle allocate VM
		if (check_resources(request, utilization[iterator_physical], H[iterator_physical])) {		
			// Allocate la VM to VM		
			if(allocate_VM_to_PM(placement, request, iterator_physical)) {
				utilization[iterator_physical][0] += request[4]*request[7]/100;
				utilization[iterator_physical][1] += request[5]*request[8]/100;
				utilization[iterator_physical][2] += request[6]*request[9]/100;
				printf("Allocated\n");
				return 1;
			}
		} 
	}
	*request_rejected = *request_rejected + 1;
	printf("NotAllocated\n");
	return 0;
}

/*
 * best_or_worst_fit: Best/First Fit Algorithm
 * parameter: logical value that indicates which strategy will be applied.
 * parameter: request of VM
 * parameter: utilization matrix	
 * parameter: placement matrix
 * parameter: H matrix
 * parameter: number of physical machines
 * returns: True (1), if the VM was correctly allocate.
 * 			False(0), other case.
 */
int best_or_worst_fit(bool is_best,float *request, float **utilization, int **placement, int **H, int h_size, int *request_rejected) {

	/* iterators */
	int iterator_physical;
	int iterator_scenario;

	PM_weight_pair_node* PM_ordered_list = (PM_weight_pair_node*)calloc(1,sizeof(PM_weight_pair_node));
	PM_ordered_list->h_index = -1;
	PM_weight_pair_node* clean_list;

	float weight_PM = 0.0;

	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
		weight_PM = calculate_weight(utilization, H[iterator_physical], iterator_physical);
		insert_PM_to_ordered_list(is_best, &PM_ordered_list, weight_PM, iterator_physical);
	}

	clean_list = PM_ordered_list;
	
	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++){
		if (check_resources(request, utilization[PM_ordered_list->h_index], H[PM_ordered_list->h_index])) {
			if(allocate_VM_to_PM(placement, request, PM_ordered_list->h_index)) {

				utilization[PM_ordered_list->h_index][0] += request[4]*request[7]/100;
				utilization[PM_ordered_list->h_index][1] += request[5]*request[8]/100;
				utilization[PM_ordered_list->h_index][2] += request[6]*request[9]/100;
			}
			free_list(clean_list);
			return 1;
		}
		PM_ordered_list = PM_ordered_list->next;
	}
	free_list(clean_list);
	return 0;
}


/*
 * best_fit: call best_or_worst_fit function with a true value. (Execute best_fit algorithm)
 * parameter: request of VM
 * parameter: utilization matrix	
 * parameter: placement matrix
 * parameter: H matrix
 * parameter: number of physical machines
 * returns: True (1), if the VM was correctly allocate.
 * 			False(0), other case.
 */
int best_fit(float *request, float **utilization, int **placement, int **H, int h_size, int *request_rejected) {
	return best_or_worst_fit(true, request, utilization, placement, H, h_size, request_rejected);
}

/*
 * worst_fit: call best_or_worst_fit function with a false value. (Execute worst_fit algorithm)
 * parameter: request of VM
 * parameter: utilization matrix	
 * parameter: placement matrix
 * parameter: H matrix
 * parameter: number of physical machines
 * returns: True (1), if the VM was correctly allocate.
 * 			False(0), other case.
 */
int worst_fit(float *request, float **utilization, int **placement, int **H, int h_size, int *request_rejected) {
	return best_or_worst_fit(false, request, utilization, placement, H, h_size, request_rejected);
}

/**
 * calculate_weight: Calculates the Weight of the physical machine
 * parameter: utilization matrix
 * parameter: H array (One physical machine)
 * parameter: number of physical machine
 * returns: Weight of the idle resources
 */
float calculate_weight(float **utilization, int *H, int h_index){
	float weight_PM = 0.0;
	weight_PM += (1.0 - utilization[h_index][0] / H[0]);
	weight_PM += (1.0 - utilization[h_index][1] / H[1]);
	weight_PM += (1.0 - utilization[h_index][2] / H[2]);
	return weight_PM;
}

/**
 * insert_PM_to_ordered_list:
 * parameter: logical value that indicates which strategy will be applied.
 * parameter: ordered list
 * parameter: weight of the physical machine
 * parameter: number of physical machine
 * returns: nothing, it is a void function
 */
void insert_PM_to_ordered_list(bool is_best, PM_weight_pair_node** PM_ordered_list, float weight_PM, int h_index) {

	bool ( *comparator )( float, float );
	PM_weight_pair_node* new_node = (PM_weight_pair_node*)calloc(1,sizeof(PM_weight_pair_node));

	new_node->weight = weight_PM;
	new_node->h_index = h_index;
	new_node->next = NULL;
	
	if(is_best){
		comparator = &best_comparator;
	}else{
		comparator = &worst_comparator;
	}

	if((*PM_ordered_list)->h_index == -1){
		*PM_ordered_list = new_node;
		return;
	}

	if((*comparator)(new_node->weight,(*PM_ordered_list)->weight)) {
		new_node->next = *PM_ordered_list;
		*PM_ordered_list = new_node;
		return;
	}

	PM_weight_pair_node* parent = *PM_ordered_list;
	PM_weight_pair_node* actual = parent->next;
	
	while(actual != NULL) {
		if((*comparator)(new_node->weight,actual->weight)){
			new_node->next = actual;
			parent->next = new_node;
			return;
		}
		parent = actual;
		actual = actual->next;
	}
	parent->next = new_node;
	return;

}


/**
 * best_comparator: Compares two weights from different physical machines
 * parameter: Weight A
 * parameter: Weight B
 * returns: True, if Weight A is bigger than Weight B
 */
bool best_comparator(float weight_A, float weight_B){
	return weight_A < weight_B;
}

/**
 * worst_comparator: Compares two weights from different physical machines
 * parameter: Weight A
 * parameter: Weight B
 * returns: True, if Weight A is smaller than Weight B
 */
bool worst_comparator(float weight_A, float weight_B){
	return weight_A > weight_B;
}

/**
 * free_list: free memory
 * parameter: list of nodes
 * returns: nothing, it is a void function
 */
void free_list(PM_weight_pair_node* list_to_free){
	PM_weight_pair_node* tmp_pointer;
	while(list_to_free != NULL){
		tmp_pointer = list_to_free;
		list_to_free = list_to_free->next;
		free(tmp_pointer);
	}
}