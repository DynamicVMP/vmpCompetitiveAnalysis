/* 
 * heuristics.c: Virtual Machine Placement Problem - Heuristics Functions Header
 * Date: February, 2016
 * Author: Rodrigo Ferreira (rodrigofepy@gmail.com) 
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */

/* include libraries */
#include "heuristics.h"

/**
 * allocate_VM_to_PM: update the placement matrix and utilization matrix, allocate VM to PM
 * parameter: placement matrix
 * parameter: request of VM
 * parameter: physical machine
 * return: True, if the VM war correctly allocate
 * 		   False, other case.	
 */
void allocate_VM_to_PM(int **placement, float **utilization, float **resources_requested, float *request, int physical_machine) {

	// Update the Placement matrix
	placement[0][(int) request[3]] = request[1]; 			// Cloud Service
	placement[1][(int) request[3]] = request[2];			// Datacenter
	placement[2][(int) request[3]] = physical_machine;		// Physical Machine
	placement[3][(int) request[3]] = request[4];			// CPU request
	placement[4][(int) request[3]] = request[5];			// RAM request
	placement[5][(int) request[3]] = request[6];			// NET request
	placement[6][(int) request[3]] = request[7];			// CPU utilization (%)
	placement[7][(int) request[3]] = request[8];			// RAM utilization (%)
	placement[8][(int) request[3]] = request[9];			// NET utilization (%)

	// Update the Utilization matrix
	utilization[physical_machine][0] += request[4]*request[7]/100;
	utilization[physical_machine][1] += request[5]*request[8]/100;
	utilization[physical_machine][2] += request[6]*request[9]/100;

	resources_requested[physical_machine][0] += request[4];
	resources_requested[physical_machine][1] += request[5];
	resources_requested[physical_machine][2] += request[6];

}

/**
 * check_resources: check the available resources of the physical machine
 * parameter: request of VM
 * parameter: array utilization
 */
bool check_resources(float *request, float *utilization, float *resources_requested, int *H) {

	return (resources_requested[0] + request[4] <= H[0]
			&& resources_requested[1] + request[5]<= H[1]
			&& resources_requested[2] + request[6]<= H[2]);

	// For overbooking
	/*return (utilization[0] + (request[4]*request[7]/100) <= H[0]
			&& utilization[1] + (request[5]*request[8]/100) <= H[1]
			&& utilization[2] + (request[6]*request[9]/100) <= H[2]);*/
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
int first_fit(float *request, float **utilization, float **resources_requested, int **placement, int **H, int h_size, int *request_rejected, VM_tend** vm_tend_list) {

	int iterator_physical;

	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
		// If request is in time 0, we directle allocate VM
		if (check_resources(request, utilization[iterator_physical], resources_requested[iterator_physical], H[iterator_physical])) {		
			// Allocate la VM to VM		
			allocate_VM_to_PM(placement, utilization, resources_requested, request, iterator_physical);
			insert_VM_to_tend_list(vm_tend_list, request, iterator_physical);
			return 1;
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
int best_or_worst_fit(bool is_best,float *request, float **utilization, float **resources_requested, int **placement, int **H, int h_size,
	
	int *request_rejected, VM_tend** vm_tend_list) {

	/* iterators */
	int iterator_physical;

	// allocate resources for a new sorted list
	PM_weight_pair_node* PM_ordered_list = (PM_weight_pair_node*)calloc(1,sizeof(PM_weight_pair_node));
	PM_ordered_list->h_index = -1;
	// pointer to keep the reference to the first element of the sorted list used to free the memory at the end of the function
	PM_weight_pair_node* clean_list;

	float weight_PM = 0.0;

	// Sort the PMs based on the weight of the idle resources
	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
		weight_PM = calculate_weight(utilization, H[iterator_physical], iterator_physical);
		insert_PM_to_ordered_list(is_best, &PM_ordered_list, weight_PM, iterator_physical);
	}

	// keep the reference to the first element of the sorted list
	clean_list = PM_ordered_list;

	// for each PM do
	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++){
		// check if the current PM of the sorted list can allocate the requested resources of the VM
		if (check_resources(request, utilization[PM_ordered_list->h_index], resources_requested[PM_ordered_list->h_index], H[PM_ordered_list->h_index])) {
			// allocate the VM into the PM
			allocate_VM_to_PM(placement, utilization, resources_requested, request, PM_ordered_list->h_index);
			insert_VM_to_tend_list(vm_tend_list, request, PM_ordered_list->h_index);
			free_list(clean_list);
			// VM allocated successfully
			return 1;
		}
		// go to next node
		PM_ordered_list = PM_ordered_list->next;
	}
	// the requested resources can't be allocated so they will be rejected
	*request_rejected = *request_rejected + 1;
	// free the allocated memory for the sorted list
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
int best_fit(float *request, float **utilization, float **resources_requested, int **placement, int **H, int h_size, int *request_rejected, VM_tend** vm_tend_list) {
	return best_or_worst_fit(true, request, utilization, resources_requested, placement, H, h_size, request_rejected, vm_tend_list);
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
int worst_fit(float *request, float **utilization, float **resources_requested, int **placement, int **H, int h_size, int *request_rejected, VM_tend** vm_tend_list) {
	return best_or_worst_fit(false, request, utilization, resources_requested, placement, H, h_size, request_rejected, vm_tend_list);
}

/**
 * calculate_weight: Calculates the Weight of the physical machine, based on the idle resources
 * parameter: utilization matrix
 * parameter: H array (One physical machine)
 * parameter: number of physical machine
 * returns: Weight of the idle resources
 */
float calculate_weight(float **utilization, int *H, int h_index) {
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

	// New Node to add
	new_node->vm_index = request[3];
	new_node->tend = request[13];
	new_node->ram_utilization = request[4]*request[7]/100;
	new_node->cpu_utilization = request[5]*request[8]/100;
	new_node->net_utilization = request[6]*request[9]/100;
	new_node->pm = physical_machine;
	new_node->next = NULL; 

	// First insert
	if((*vm_tend_list)->vm_index == -1){
		*vm_tend_list = new_node;
		return;
	}

	// First element of the list
	if(time_comparator(new_node->tend,(*vm_tend_list)->tend)) {
		new_node->next = *vm_tend_list;
		*vm_tend_list = new_node;
		return;
	}

	VM_tend* parent = *vm_tend_list;
	VM_tend* actual = parent->next;

	// Iterate until find the place of the new node
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
 * insert_PM_to_ordered_list: insert a given PM inside a ordered list
 * parameter is_best: logical value that indicates which strategy will be applied. True for best_fit, False for worst_fit
 * parameter PM_ordered_list: ordered list of al the PMs, in the first iteration this will be null
 * parameter weight_PM: weight of the physical machine
 * parameter h_index: index of the physical machine
 * returns: nothing, it is a void function
 */
void insert_PM_to_ordered_list(bool is_best, PM_weight_pair_node** PM_ordered_list, float weight_PM, int h_index) {

	bool ( *comparator )( float, float );
	PM_weight_pair_node* new_node = (PM_weight_pair_node*)calloc(1,sizeof(PM_weight_pair_node));

	// Set the new node
	new_node->weight = weight_PM;
	new_node->h_index = h_index;
	new_node->next = NULL;

	// select the comparator to use
	if(is_best){
		comparator = &best_comparator;
	}else{
		comparator = &worst_comparator;
	}

	// if the ordered_list have the index -1 means that the list is empty, so we insert the new node in the first pointer
	if((*PM_ordered_list)->h_index == -1){
		*PM_ordered_list = new_node;
		return;
	}

	// if the comparation between the new node and the first item in the ordered list returns true, the new node will be
	// inserted before the actual head of the list
	if((*comparator)(new_node->weight,(*PM_ordered_list)->weight)) {
		new_node->next = *PM_ordered_list;
		*PM_ordered_list = new_node;
		return;
	}

	// set the initial configuration to iterate through the ordered list
	PM_weight_pair_node* parent = *PM_ordered_list;
	PM_weight_pair_node* actual = parent->next;

	// ensure that the actual value isn't null
	while(actual != NULL) {
		// compare the new node with all the elements of the list to found his place
		if((*comparator)(new_node->weight,actual->weight)){
			new_node->next = actual;
			parent->next = new_node;
			return;
		}
		// go to the next element
		parent = actual;
		actual = actual->next;
	}
	// the new node will be inserted at the bottom of the list
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
		// split all the requests in groups of same T, to avoid unnecessary comparisons of the sort algorithm
		if (S[temp_t][0] < S[index][0]) {
			quicksort_decreasing_sort(S, temp_t, index - 1);
			temp_t = index;
		} else if (index == s_size - 1) {
			quicksort_decreasing_sort(S, temp_t, index);
		}
	}

}

/**
 * quicksort_decreasing_sort: Sort the Scenario with quicksort in-place based on representative weights
 * parameter S: Scenario to sort
 * parameter first_index: index of the first element in S
 * parameter last_index: index of the last element in S
 *
 * returns: nothing, it's a void function.
 */
void quicksort_decreasing_sort(float **S, int first_index, int last_index){

	int pivot_index = 0;

	if(first_index < last_index) {
		pivot_index = quicksort_partition(S, first_index, last_index);
		quicksort_decreasing_sort(S, first_index, (pivot_index - 1));
		quicksort_decreasing_sort(S, (pivot_index + 1), last_index);
	}

}

/**
 * quicksort_partition: partition of the quicksort algorithm
 * parameter S: Scenario to sort
 * parameter first_index: index of the first element in S
 * parameter last_index: index of the last element in S
 *
 * returns: the position of the pivot to use.
 */
int quicksort_partition(float **S, int first_index, int last_index) {

	int up_index, down_index;
	bool first_iteration = true;
	float * temp = (float *) calloc (sizeof (float), 14);
	float * pivot = (float *) calloc (sizeof (float), 14);

	memcpy(pivot, S[first_index], 14 * sizeof(float));
	up_index = first_index;
	down_index = last_index;

	do {
		// If is not the first iteration, to avoid the use of goto statements
		if(!first_iteration) {
			memcpy(temp, S[up_index], 14 * sizeof(float));
			memcpy(S[up_index], S[down_index], 14 * sizeof(float));
			memcpy(S[down_index], temp, 14 * sizeof(float));
		}
		first_iteration = false;

		// While the up element is better or equal than the selected pivot
		while (compare_requests(S[up_index], pivot) != -1 && up_index < last_index) {
			up_index++;
		}

		// While the down element is worst than the selected pivot
		while (compare_requests(S[down_index], pivot) == -1  && down_index > first_index ) {
			down_index--;
		}

	} while (down_index > up_index);

	if(compare_requests(S[first_index], S[down_index]) == -1) {
		memcpy(S[first_index], S[down_index], 14 * sizeof(float));
		memcpy(S[down_index], pivot, 14 * sizeof(float));
	}

	// free allocated memory
	free(temp);
	free(pivot);

	return down_index;

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
int compare_requests(float* request_A, float* request_B) {
	// Coefficients defined to make equal-significant the variation of the requested resources
	float alpha = 1.0, beta = 1.0, gamma = 1.0;
	// The average value of the requests A and B
	float representative_weight_A, representative_weight_B;
	// If the request A occurs before B, A is better than B
	if(request_A[0] < request_B[0]) {
		return 1;
	} if(request_A[0] > request_B[0]) {
		return -1;
	}

	// The sum of all the resources multiplied by their coefficient divided by the number of resources
	representative_weight_A = (request_A[4] * alpha + request_A[5] * beta + request_A[6] * gamma) / 3;
	representative_weight_B = (request_B[4] * alpha + request_B[5] * beta + request_B[6] * gamma) / 3;

	if(representative_weight_A > representative_weight_B) {
		return 1;
	} else if (representative_weight_A == representative_weight_B) {
		return 0;
	}
	// B is better than A
	return -1;
}

/**
 * time_comparator
 * parameter: time_A [description]
 * parameter: time_B [description]
 * return:  True, is time A is less than time B.
 * 			False, other case.
 */
bool time_comparator(int time_A, int time_B) {
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
void remove_VM_from_placement(VM_tend** vm_tend_list, int **placement, float **utilization, float **resources_requested, int current_time,
	int h_size) {

	VM_tend* parent = *vm_tend_list;
	while(parent->tend == current_time) {
		// update the placement matrix and set -1 PM for the VM
		placement[2][parent->vm_index] = -1;
		// update the utilization matrix
		utilization[parent->pm][0] -= placement[3][parent->vm_index]*placement[6][parent->vm_index]/100;
		utilization[parent->pm][1] -= placement[4][parent->vm_index]*placement[7][parent->vm_index]/100;
		utilization[parent->pm][2] -= placement[5][parent->vm_index]*placement[8][parent->vm_index]/100;

		resources_requested[parent->pm][0] -= placement[3][parent->vm_index];
		resources_requested[parent->pm][1] -= placement[4][parent->vm_index];
		resources_requested[parent->pm][2] -= placement[5][parent->vm_index];

		// Delete node from ordered list
		VM_tend* temp = *vm_tend_list;
		*vm_tend_list = (*vm_tend_list)->next;
		parent = *vm_tend_list;
		free(temp);
    }
}

/**
 * update_VM_resources: Update VM resources.
 * parameter placement   Placement matrix
 * parameter utilization Utilization matrix
 * parameter request     Request to process
 * parameter vm_list     List of VMs
 * parameter H           List of PMs
 * return 	True, if the VM does not need a migration
 * 			False, other case
 */
bool update_VM_resources(int **placement, float **utilization, float **resources_requested, float *request, VM_tend** vm_list, int **H) {

	int physical_machine = placement[2][(int) request[3]];
	float *temp_request = (float *) malloc (14 *sizeof (float));
	int iterator, aux;

	memcpy(temp_request, request, 14 * sizeof(float));

	// Create a temp_request to verify if the physical machine can hold  VM's new resources.
	for (iterator = 4; iterator < 7; iterator++) {
		aux	= iterator - 1;
		if(request[iterator] > placement[aux][(int) request[3]] ) {
			temp_request[iterator] = request[iterator] - placement[aux][(int) request[3]];
		} else {
			temp_request[iterator] = placement[aux][(int) request[3]] - request[iterator];
		}
	}

	// Verify if the PM can hold the VM's new resources.
	if(check_resources(temp_request, utilization[physical_machine], resources_requested[physical_machine],  H[physical_machine])) {
		free(temp_request);
		utilization[physical_machine][0] -= placement[3][(int) request[3]]*placement[6][(int) request[3]]/100;
		utilization[physical_machine][1] -= placement[4][(int) request[3]]*placement[7][(int) request[3]]/100;
		utilization[physical_machine][2] -= placement[5][(int) request[3]]*placement[8][(int) request[3]]/100;

		resources_requested[physical_machine][0] -= placement[3][(int) request[3]];
		resources_requested[physical_machine][1] -= placement[4][(int) request[3]];
		resources_requested[physical_machine][2] -= placement[5][(int) request[3]];

		allocate_VM_to_PM(placement, utilization, resources_requested, request, physical_machine);
		return true;
	} else {
		free(temp_request);
		// For migration
		// if the PM can't hold the VM's new resources, the VM must be allocate to another PM
		// placement[2][(int) request[3]] = -1;	 	// Physical Machine
		return false;
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

