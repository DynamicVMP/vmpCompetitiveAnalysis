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



// ############################################ Heuristics ############################################### //
/*
 * first_fit: First Fit Algorithm
 * parameter: request of VM
 * parameter: utilization matrix	
 * parameter: placement matrix
 * parameter: number of physical machines
 * returns: True (1), if the VM was correctly allocate.
 * 			False(0), other case.
 */
int first_fit(float *request, float **utilization, int **placement, int **H, int h_size, int *request_rejected, VM_tend** vm_tend_list) {

	int iterator_physical; 

	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
		// If request is in time 0, we directle allocate VM
		if (check_resources(request, utilization[iterator_physical], H[iterator_physical])) {		
			// Allocate la VM to VM		
			if(allocate_VM_to_PM(placement, request, iterator_physical)) {
				utilization[iterator_physical][0] += request[4]*request[7]/100;
				utilization[iterator_physical][1] += request[5]*request[8]/100;
				utilization[iterator_physical][2] += request[6]*request[9]/100;
				insert_VM_to_tend_list(vm_tend_list, request, iterator_physical);
				return 1;
			}
		} 
	}
	*request_rejected = *request_rejected + 1;
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
int best_or_worst_fit(bool is_best,float *request, float **utilization, int **placement, int **H, int h_size, int *request_rejected, VM_tend** vm_tend_list) {

	/* iterators */
	int iterator_physical;

	PM_weight_pair_node* PM_ordered_list = (PM_weight_pair_node*)calloc(1,sizeof(PM_weight_pair_node));
	PM_ordered_list->h_index = -1;
	PM_weight_pair_node* clean_list;

	float weight_PM = 0.0;

	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
		weight_PM = calculate_weight(utilization, H[iterator_physical], iterator_physical);
		insert_PM_to_ordered_list(is_best, &PM_ordered_list, weight_PM, iterator_physical);
		//print_PM_list(PM_ordered_list);
	}
 
	clean_list = PM_ordered_list;
	
	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++){
		if (check_resources(request, utilization[PM_ordered_list->h_index], H[PM_ordered_list->h_index])) {
			if(allocate_VM_to_PM(placement, request, PM_ordered_list->h_index)) {

				utilization[PM_ordered_list->h_index][0] += request[4]*request[7]/100;
				utilization[PM_ordered_list->h_index][1] += request[5]*request[8]/100;
				utilization[PM_ordered_list->h_index][2] += request[6]*request[9]/100;
				insert_VM_to_tend_list(vm_tend_list, request, iterator_physical);
			}
			free_list(clean_list);
			return 1;
		}
		PM_ordered_list = PM_ordered_list->next;
	}
	*request_rejected = *request_rejected + 1;
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
int best_fit(float *request, float **utilization, int **placement, int **H, int h_size, int *request_rejected, VM_tend** vm_tend_list) {
	return best_or_worst_fit(true, request, utilization, placement, H, h_size, request_rejected, vm_tend_list);
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
int worst_fit(float *request, float **utilization, int **placement, int **H, int h_size, int *request_rejected, VM_tend** vm_tend_list) {
	return best_or_worst_fit(false, request, utilization, placement, H, h_size, request_rejected, vm_tend_list);
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
 * insert_VM_to_VMtend_list: Insert VM to List of VM 
 * parameter VM_tend_list: 	List of nodes 	
 * parameter vm_index:	VM index
 */
void insert_VM_to_tend_list(VM_tend** vm_tend_list, float *request, int physical_machine) {

	VM_tend* new_node = (VM_tend*)calloc(1,sizeof(VM_tend));

	new_node->vm_index = request[3];
	new_node->tend = request[13];
	new_node->ram_utilization = request[4]*request[7]/100;
	new_node->cpu_utilization = request[5]*request[8]/100;
	new_node->net_utilization = request[6]*request[9]/100;
	new_node->pm = physical_machine;
	new_node->next = NULL; 

	// first insert
	if((*vm_tend_list)->vm_index == -1){
		*vm_tend_list = new_node;
		return;
	}

	if(time_comparator(new_node->tend,(*vm_tend_list)->tend)) {
		new_node->next = *vm_tend_list;
		*vm_tend_list = new_node;
		return;
	}

	VM_tend* parent = *vm_tend_list;
	VM_tend* actual = parent->next;

	while(actual != NULL) {
		if(time_comparator(new_node->tend,actual->tend)){
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
 * returns: True, if Weight A is less than Weight B
 */
bool best_comparator(float weight_A, float weight_B){
	return weight_A < weight_B;
}

/**
 * worst_comparator: Compares two weights from different physical machines
 * parameter: Weight A
 * parameter: Weight B
 * returns: True, if Weight A is greater than Weight B
 */
bool worst_comparator(float weight_A, float weight_B){
	return weight_A > weight_B;
}

/**
 * prepare_input_for_decreasing_heuristics: sort the list of requests in decreasing order
 * parameter S: list of requests in the Scenario
 * parameter s_size: number of requests in the Scenario
 * return: nothing, it's a void	function
 */
void prepare_input_for_decreasing_heuristics(float **S, int s_size) {

	int index, temp_t = 0;

	for (index = 1; index < s_size; index++) {
		if (S[temp_t][0] < S[index][0]) {
			printf("for and if\n");
			quicksort_decreasing_sort(S, temp_t, index - 1);
			temp_t = index;
		} else if (index == s_size - 1) {
			printf("for and elseif\n");
			quicksort_decreasing_sort(S, temp_t, index);
		}
	}

	for (index = 0; index < s_size; index++) {
		printf("VM: %f\n", S[index][3]);
	}
}

/**
 *
 */

void quicksort_decreasing_sort(float **S, int first_index, int last_index){

	if(first_index < last_index){
		int pivot, from_last_index, from_first_index;
		float * temp_request = (float *) calloc (sizeof (float), 14);

		pivot = first_index;
		from_first_index = first_index;
		from_last_index = last_index;
		while(from_first_index < from_last_index){
			while(compare_requests(S[from_first_index], S[pivot]) != -1 && from_first_index < last_index)
				from_first_index++;
			while(compare_requests(S[from_last_index], S[pivot]) != 1 && from_first_index < last_index)
				from_last_index--;
			if(from_first_index < from_last_index){
				memcpy(temp_request, S[from_first_index], 14 * sizeof(float));
				memcpy(S[from_first_index], S[from_last_index], 14 * sizeof(float));
				memcpy(S[from_last_index], temp_request, 14 * sizeof(float));
			}
		}

		memcpy(temp_request, S[pivot], 14 * sizeof(float));
		memcpy(S[pivot], S[from_last_index], 14 * sizeof(float));
		memcpy(S[from_last_index], temp_request, 14 * sizeof(float));

		quicksort_decreasing_sort(S, first_index, from_last_index - 1);
		quicksort_decreasing_sort(S, from_last_index + 1, last_index);

		free(temp_request);
	}

}

/**
 * is_better_than: Compare two requests, is used in decreasing algorithms
 * parameter request_A: the request to be compared
 * parameter request_B: the request to be compared
 *
 * returns: 1, if request A is better than request B
 * 			0, if A and B are equals
 * 			-1, if B is better than A
 */
int compare_requests(float* request_A, float* request_B){
	// Coefficients defined to make equal-significant the variation of the requested resources
	float alpha = 1.0, beta = 1.0, gamma = 1.0;
	// The average value of the requests A and B
	float representative_weight_A, representative_weight_B;
	// If the request A occurs before B, A is better than B
	if(request_A[0] < request_B[0]){
		return 1;
	}if(request_A[0] > request_B[0]){
		return -1;
	}

	// The sum of all the resources multiplied by their coefficient divided by the number of resources
	representative_weight_A = (request_A[4] * alpha + request_A[5] * beta + request_A[6] * gamma) / 3;
	representative_weight_B = (request_B[4] * alpha + request_B[5] * beta + request_B[6] * gamma) / 3;

	if(representative_weight_A > representative_weight_B){
		return 1;
	} else if (representative_weight_A == representative_weight_B){
		return 0;
	}
	// B is better than A
	return -1;
}

bool time_comparator(int time_A, int time_B){
	return time_A < time_B;
}

/**
 * remove_VM_from_placement: Remove the VM from Placement matrix and update the Utilization matrix
 * parameter vm_to_remove: VM to shut down
 * parameter placement:	Placement matrix
 * parameter utilization: Utilization matrix
 * parameter current_time: Current time
 * parameter h_size: Number of physical machines
 * return: nothing, it's a void function.
 */
void remove_VM_from_placement(VM_tend* vm_to_remove, int **placement, float **utilization, int current_time, 
	int h_size) {
	VM_tend* tmp_pointer;
	int iterator_physical;
	
	while(vm_to_remove != NULL) {
		tmp_pointer = vm_to_remove;
		vm_to_remove = vm_to_remove->next;

		// check tend of the VM
		if(tmp_pointer->tend == current_time) {			
			// update the placement matrix
			for(iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
				placement[tmp_pointer->vm_index][iterator_physical] = 0;
			}	
			// update the utilization matrix
			utilization[tmp_pointer->pm][0] -= tmp_pointer->ram_utilization;
			utilization[tmp_pointer->pm][1] -= tmp_pointer->cpu_utilization;
			utilization[tmp_pointer->pm][2] -= tmp_pointer->net_utilization;	
		}
	}
}


// ############################################ Clean Memory ############################################# //
/**
 * free_list: free memory
 * parameter: list of nodes
 * returns: nothing, it is a void function
 */
void free_VM_list(VM_tend* list_to_free) {
	VM_tend* tmp_pointer;
	while(list_to_free != NULL){
		tmp_pointer = list_to_free;
		list_to_free = list_to_free->next;
		free(tmp_pointer);
	}
}

/**
 * free_list: free memory
 * parameter: list of nodes
 * returns: nothing, it is a void function
 */
void free_list(PM_weight_pair_node* list_to_free) {
	PM_weight_pair_node* tmp_pointer;
	while(list_to_free != NULL){
		tmp_pointer = list_to_free;
		list_to_free = list_to_free->next;
		free(tmp_pointer);
	}
}

// ############################################## Print ################################################## //

/**
 * print_PM_list description: Print all the VMs in the list
 * parameter: List of nodes
 * returns: nothing, it's a void function.
 */
void print_VM_list(VM_tend* list) {
	VM_tend* tmp_pointer;
	printf("\n");
	while(list != NULL){
		tmp_pointer = list;
		list = list->next;
		printf("(VM: %d, ", tmp_pointer->vm_index);
		printf("PM: %d, ", tmp_pointer->pm);
		printf("Ttend: %d)\t", tmp_pointer->tend);
		printf("->");
	}
	printf("NULL\n");
}

/**
 * print_PM_list description: Print all the PMs in the list
 * parameter: List of nodes
 * returns: nothing, it's a void function.
 */
void print_PM_list(PM_weight_pair_node* list) {
	PM_weight_pair_node* tmp_pointer;
	printf("\n");
	while(list != NULL){
		tmp_pointer = list;
		list = list->next;
		printf("(PM: %d, ", tmp_pointer->h_index);
		printf("Weight: %f)\t", tmp_pointer->weight);
		printf("->");
	}
	printf("NULL\n");
}