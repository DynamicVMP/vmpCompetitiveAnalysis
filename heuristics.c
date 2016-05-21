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
 *
 * parameter placement           Placement matrix
 * parameter utilization         Utilization matrix
 * parameter resources_requested Resources requested matrix
 * parameter request             Request
 * parameter physical_machine    PM id
 * 
 * return: nothing, it is a void function	
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
	placement[9][(int) request[3]] = request[10];			// Revenue
	placement[10][(int) request[3]] = request[11];			// SLA

	// Update the Utilization matrix
	utilization[physical_machine][0] += request[4]*request[7]/100;
	utilization[physical_machine][1] += request[5]*request[8]/100;
	utilization[physical_machine][2] += request[6]*request[9]/100;

	resources_requested[physical_machine][0] = resources_requested[physical_machine][0] + request[4];
	resources_requested[physical_machine][1] = resources_requested[physical_machine][1] + request[5];
	resources_requested[physical_machine][2] = resources_requested[physical_machine][2] + request[6];
}

/**
 * check_resources: check the available resources of the physical machine
 *
 * parameter: request             request of VM
 * parameter: utilization         Utilization matrix
 * parameter: resources_requested Resources requested matrix
 * parameter: H                   PM matrix
 * parameter: vm_list             List of VM
 * parameter: physical_machine    PM id
 * parameter: update              True, if the VM update his resources
 * 								 False, other case
 *
 * return True, if the current PM can hold the VM.
 * 		  False, othre case
 */
bool check_resources(float *request, float **utilization, float **resources_requested, int **H, VM_linked_list** vm_list, int physical_machine, bool update) {

	bool flag = (resources_requested[physical_machine][0] + request[4] <= H[physical_machine][0]
				 && resources_requested[physical_machine][1] + request[5]<= H[physical_machine][1]
				 && resources_requested[physical_machine][2] + request[6]<= H[physical_machine][2]);

	if(!flag) {
		return flag;
	} 

	if(!update) {
		VM_linked_list* parent = *vm_list;
		
		do {
			if(parent->service == (int) request[1] && parent->pm == physical_machine) {
				return false;
			}
			parent = parent->next;
		} while(parent != NULL);	
	}
	
	return flag;

	// For overbooking
	/*return (utilization[0] + (request[4]*request[7]/100) <= H[0]
			&& utilization[1] + (request[5]*request[8]/100) <= H[1]
			&& utilization[2] + (request[6]*request[9]/100) <= H[2]);*/
}

// ################################################# Heuristics #################################################### //

/**
 * first_fit: First Fit Algorithm
 *
 * parameter: request             Request of VM
 * parameter: utilization         Utilization matrix
 * parameter: resources_requested Resources requested matrix
 * parameter: placement           Placement matrix
 * parameter: H                   PMs matrix
 * parameter: h_size              Number of PMs
 * parameter: request_rejected    Number of request rejected
 * parameter: vm_list        List of VM
 * parameter: migration           True, if is a migration between PMs
 * 								 False, other case
 * 
 * return: 	True (1), if the VM was correctly allocate.
 * 			False(0), other case.
 */
bool first_fit(float *request, float **utilization, float **resources_requested, int **placement, int **H, int h_size, VM_linked_list** VM_list_derived, VM_linked_list** vm_list, VM_linked_list** VM_list_serviced, VM_linked_list** VM_list_serviced_derived) {

	int iterator_physical;

	for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
		// If request is in time 0, we directle allocate VM
		if (check_resources(request, utilization, resources_requested, H, vm_list, iterator_physical, false )) {		
			// Allocate la VM to VM		
			allocate_VM_to_PM(placement, utilization, resources_requested, request, iterator_physical);
			insert_VM_to_tend_list(vm_list, request, iterator_physical);
			insert_VM_to_tend_list(VM_list_serviced, request, iterator_physical);
			return true;
		} 
	}
	insert_VM_to_tend_list(VM_list_serviced_derived, request, iterator_physical);
	insert_VM_to_tend_list(VM_list_derived, request, iterator_physical);
	return false;
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

bool best_or_worst_fit(bool is_best,float *request, float **utilization, float **resources_requested, int **placement,
					  int **H, int h_size, VM_linked_list** VM_list_derived, VM_linked_list** vm_list, VM_linked_list** VM_list_serviced, VM_linked_list** VM_list_serviced_derived) {

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
		if (check_resources(request, utilization, resources_requested, H, vm_list, PM_ordered_list->h_index, false)) {

			// allocate the VM into the PM
			allocate_VM_to_PM(placement, utilization, resources_requested, request, PM_ordered_list->h_index);
			insert_VM_to_tend_list(vm_list, request, PM_ordered_list->h_index);
			insert_VM_to_tend_list(VM_list_serviced, request, iterator_physical);
			free_list(clean_list);
			// VM allocated successfully
			return true;
		}
		// go to next node
		PM_ordered_list = PM_ordered_list->next;
	}
	insert_VM_to_tend_list(VM_list_serviced_derived, request, iterator_physical);
	insert_VM_to_tend_list(VM_list_derived, request, iterator_physical);
	// free the allocated memory for the sorted list
	free_list(clean_list);

	return false;
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
bool best_fit(float *request, float **utilization, float **resources_requested, int **placement, int **H, int h_size, VM_linked_list** VM_list_derived, VM_linked_list** vm_list, VM_linked_list** VM_list_serviced, VM_linked_list** VM_list_serviced_derived) {
	return best_or_worst_fit(true, request, utilization, resources_requested, placement, H, h_size, VM_list_derived, vm_list, VM_list_serviced, VM_list_serviced_derived);
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
bool worst_fit(float *request, float **utilization, float **resources_requested, int **placement, int **H, int h_size, VM_linked_list** VM_list_derived, VM_linked_list** vm_list, VM_linked_list** VM_list_serviced, VM_linked_list** VM_list_serviced_derived) {
	return best_or_worst_fit(false, request, utilization, resources_requested, placement, H, h_size, VM_list_derived, vm_list, VM_list_serviced, VM_list_serviced_derived);
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
 * 
 * parameter vm_list:     	List of nodes
 * parameter request:         	Request 
 * parameter physical_machine: 	PM id
 */
void insert_VM_to_tend_list(VM_linked_list** vm_list, float *request, int physical_machine) {

	VM_linked_list* new_node = (VM_linked_list*)calloc(1,sizeof(VM_linked_list));

	// New Node to add
	new_node->vm_index = request[3];						// VM id
	new_node->service = request[1];							// Service
	new_node->datacenter = request[2];						// Datacenter
	new_node->tend = request[13];							// t end
	new_node->cpu = request[4];
	new_node->ram = request[5];
	new_node->net = request[6];
	new_node->cpu_utilization = request[4]*request[7]/100;	// CPU Utilization
	new_node->ram_utilization = request[5]*request[8]/100;	// RAM Utilization
	new_node->net_utilization = request[6]*request[9]/100;  // NET Utilization
	new_node->revenue = request[10];						// Revenue
	new_node->pm = physical_machine;						// PM allocated
	new_node->SLA = request[11];							// SLA
	new_node->next = NULL; 									// pointer to next node

	// First insert
	if((*vm_list)->vm_index == -1){
		*vm_list = new_node;
		return;
	}

	// First element of the list
	if(time_comparator(new_node->tend,(*vm_list)->tend)) {
		new_node->next = *vm_list;
		*vm_list = new_node;
		return;
	}

	VM_linked_list* parent = *vm_list;
	VM_linked_list* actual = parent->next;

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
	// the new node will be inserted at the bottom of the list
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
 * time_comparator: Compares two times
 * parameter: time_A 
 * parameter: time_B 
 * return:  True, is time A is less than time B.
 * 			False, other case.
 */
bool time_comparator(int time_A, int time_B) {
	return time_A < time_B;
}

/**
 * remove_VM_by_time: Remove the VM from Placement matrix and update the Utilization matrix
 * parameter vm_to_remove: VM to shut down
 * parameter placement:	Placement matrix
 * parameter utilization: Utilization matrix
 * parameter current_time: Current time
 * parameter h_size: Number of physical machines
 * return: nothing, it's a void function.
 */
long remove_VM_by_time(VM_linked_list** vm_list, VM_linked_list** VM_list_derived, int **placement, float **utilization, float **resources_requested, int current_time,
	int h_size) {

	long lost_revenue = 0;

	VM_linked_list* parent = *vm_list;
	VM_linked_list* actual = parent->next;

	while(actual != NULL) { 
		if(parent->tend != -1 && parent->tend < current_time) {

			// update the utilization matrix
			utilization[parent->pm][0] -= parent->cpu_utilization;
			utilization[parent->pm][1] -= parent->ram_utilization;
			utilization[parent->pm][2] -= parent->net_utilization;

			resources_requested[parent->pm][0] = resources_requested[parent->pm][0] - (float) parent->cpu;
			resources_requested[parent->pm][1] = resources_requested[parent->pm][1] - (float) parent->ram;
			resources_requested[parent->pm][2] = resources_requested[parent->pm][2] - (float) parent->net;

			lost_revenue = lost_revenue + parent->revenue;

			VM_linked_list* temp = *vm_list;
			*vm_list = (*vm_list)->next;
			parent = *vm_list;
			free(temp);

	    }
	    parent = actual;
	    actual = actual->next;
	}

	// Remove unique node
	if(parent->tend != -1 && parent->tend < current_time) {
		// update the utilization matrix
		utilization[parent->pm][0] -= parent->cpu_utilization;
		utilization[parent->pm][1] -= parent->ram_utilization;
		utilization[parent->pm][2] -= parent->net_utilization;

		resources_requested[parent->pm][0] = resources_requested[parent->pm][0] - (float) parent->cpu;
		resources_requested[parent->pm][1] = resources_requested[parent->pm][1] - (float) parent->ram;
		resources_requested[parent->pm][2] = resources_requested[parent->pm][2] - (float) parent->net;

		lost_revenue = lost_revenue + parent->revenue;

		parent->vm_index = -1;
		parent->tend = -1;
		parent->pm = -1;
		parent->SLA = 0;
		parent->revenue = 0;
		parent->next = NULL;
    }
	
	VM_linked_list* parent_derived = *VM_list_derived;
	VM_linked_list* actual_derived = parent_derived->next;

	while(actual_derived != NULL) { 
		if(parent_derived->tend != -1 && parent_derived->tend < current_time) {
			
			lost_revenue = lost_revenue + parent_derived->revenue;

			VM_linked_list* temp = *VM_list_derived;
			*VM_list_derived = (*VM_list_derived)->next;
			parent_derived = *VM_list_derived;
			free(temp);
	    
	    }
	    parent_derived = actual_derived;
	    actual_derived = actual_derived->next;
	}

	// Remove unique node
	if(parent_derived->tend != -1 && parent_derived->tend < current_time) {
		lost_revenue = lost_revenue + parent_derived->revenue;
		parent_derived->vm_index = -1;
		parent_derived->tend = -1;
		parent_derived->pm = -1;
		parent_derived->SLA = 0;
		parent_derived->revenue = 0;
		parent_derived->next = NULL;
    }

	return lost_revenue;

}


/**
 * remove_VM_by_time: Remove the VM from Placement matrix and update the Utilization matrix
 * parameter vm_to_remove: VM to shut down
 * parameter placement:	Placement matrix
 * parameter utilization: Utilization matrix
 * parameter current_time: Current time
 * parameter h_size: Number of physical machines
 * return: nothing, it's a void function.
 */
void update_VM_list(VM_linked_list** vm_list, float *request, int physical_machine) {

	VM_linked_list* parent = *vm_list;

	do {
		if( parent->service == request[1] && parent->datacenter == request[2] && parent->vm_index == request[3]) { 
			parent->vm_index = request[3];							// VM id
			parent->service = request[1];							// Service
			parent->datacenter = request[2];						// Datacenter
			parent->tend = request[13];								// t end
			parent->cpu = request[4];
			parent->ram = request[5];
			parent->net = request[6];
			parent->cpu_utilization = request[4]*request[7]/100;	// CPU Utilization
			parent->ram_utilization = request[5]*request[8]/100;	// RAM Utilization
			parent->net_utilization = request[6]*request[9]/100;  	// NET Utilization
			parent->revenue = request[10];							// Revenue
			parent->SLA = request[11];								// SLA
		}
		parent = parent->next;

	} while(parent != NULL);
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
bool update_VM_resources(int **placement, float **utilization, float **resources_requested, float *request, VM_linked_list** vm_list, int **H) {

	float *temp_request = (float *) malloc (14 *sizeof (float));
	int iterator;
	memcpy(temp_request, request, 14 * sizeof(float));

	VM_linked_list* parent = *vm_list;
	VM_linked_list* actual = parent->next;

	int physical_machine = -1;

	while(actual != NULL )  {	
		if( parent->service == request[1] && parent->datacenter == request[2] && parent->vm_index == request[3]) { 
			physical_machine = parent->pm;
			break;
		}
		parent = actual;
		actual = actual->next;	
	}

	if(request[4] > parent->cpu ) {
		temp_request[4] = request[4] -parent->cpu;
	} else {
		temp_request[4] = parent->cpu - request[4];
	}

	if(request[5] > parent->ram ) {
		temp_request[5] = request[5] -parent->ram;
	} else {
		temp_request[5] = parent->ram - request[5];
	}

	if(request[6] > parent->net ) {
		temp_request[6] = request[6] -parent->net;
	} else {
		temp_request[6] = parent->net - request[6];
	}
	
	if(physical_machine == -1) {
		free(temp_request);
		// printf("THE CURRENT VM IS NOT IN THE CURRENT DATACENTER\n");
		return false;
	}

	//Verify if the PM can hold the VM's new resources.
	if(check_resources(temp_request, utilization, resources_requested,  H, vm_list, physical_machine, true)) {
		free(temp_request);
		utilization[physical_machine][0] -= parent->cpu_utilization;
		utilization[physical_machine][1] -= parent->ram_utilization;
		utilization[physical_machine][2] -= parent->net_utilization;

		resources_requested[physical_machine][0] -= parent->cpu;
		resources_requested[physical_machine][1] -= parent->ram;
		resources_requested[physical_machine][2] -= parent->net;

		// Update node from VM list 
		update_VM_list(vm_list, request, physical_machine);
		allocate_VM_to_PM(placement, utilization, resources_requested, request, physical_machine);
		return true;
	} else {
		free(temp_request);
		return false;
	}
}


// ############################################ Clean Memory ############################################# //
/**
 * free_list: free memory
 * parameter: list of nodes
 * returns: nothing, it is a void function
 */
void free_VM_list(VM_linked_list* list_to_free) {

	VM_linked_list* tmp_pointer;
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
void print_VM_list(VM_linked_list* list) {

	VM_linked_list* tmp_pointer;
	printf("\n");
	while(list != NULL){
		tmp_pointer = list;
		list = list->next;
		printf("(Sb: %d, ", tmp_pointer->service);
		printf("DC: %d, ", tmp_pointer->datacenter);
		printf("VM: %d, ", tmp_pointer->vm_index);
		printf("PM: %d, ", tmp_pointer->pm);
		printf("Tend: %d, ", tmp_pointer->tend);
		printf("SLA: %d, ", tmp_pointer->SLA);
		printf("R: %li, ", tmp_pointer->revenue);

		printf("CPU: %d, ", tmp_pointer->cpu);
		printf("RAM: %d, ", tmp_pointer->ram);
		printf("NET: %d, ", tmp_pointer->net);

		printf("CPU_UTIL: %g, ", tmp_pointer->cpu_utilization);
		printf("RAM_UTIL: %g, ", tmp_pointer->ram_utilization);
		printf("NET_UTIL: %g) ->\n ", tmp_pointer->net_utilization);
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

// ############################################## F.O. ################################################## //

/**
 * economical_revenue: Calculates the total revenue and Quality of Service per VM allocated.
 *
 * parameter: VM_list  List of VMs allocated
 * parameter: total_revenue Revenue 
 * parameter: total_qos     Quality of Service
 * return: nothing, it's a void function
 */
void economical_revenue (VM_linked_list** vm_list, VM_linked_list** VM_list_derived, long *total_revenue, long *total_qos, int *living_vms, int *living_derived_vms) {
	
	*total_revenue = 0;
	*total_qos = 0;

	*living_vms = 0;
	*living_derived_vms = 0;

	VM_linked_list* parent = *vm_list;
	do {
		*living_vms = *living_vms + 1;
		*total_revenue = *total_revenue + parent->revenue;
		*total_qos = *total_qos + (custom_pow(CONSTANT,parent->SLA) * parent->SLA);
		parent = parent->next;
	} while(parent != NULL);
		

	// VMs Derived
	VM_linked_list* parent_derived = *VM_list_derived;
	do {
		*living_derived_vms = *living_derived_vms + 1;
		*total_revenue = *total_revenue + parent_derived->revenue * 0.3;
		
		parent_derived = parent_derived->next;
	} while(parent_derived != NULL);

}

/**
 * wasted_resources: Calculates the ratio of the wasted resources of all the working PMs
 *
 * parameter: utilization			Utilization matrix
 * parameter: resources_requested 	resource request matrix
 * parameter: H     				Array of PMs
 * parameter: h_size				Number of PMs
 * return: wasted_resources_ratio
 */
float wasted_resources (float **utilization, float **resources_requested, int **H, int h_size, float *wasted_resources_obj) {

	float wasted_cpu_resources = 0.0 , wasted_ram_resources = 0.0 , wasted_net_resources = 0.0;
	float wasted_cpu_resources_ratio = 0.0 , wasted_ram_resources_ratio = 0.0 , wasted_net_resources_ratio = 0.0;
	float alpha = 1.0, beta = 1.0, gamma = 1.0, wasted_resources_ratio = 0.0;
	int working_pms = 0, pm_index;

	// for all the PMs do
	for (pm_index = 0; pm_index < h_size; ++pm_index) {
		// if the PM has at least one VM allocated, it's considered a working PM
		if(resources_requested[pm_index][0] != 0 || resources_requested[pm_index][1] != 0 || resources_requested[pm_index][2] != 0){
			working_pms++;
			// sum of all the wasted resources of all the PMs
			wasted_cpu_resources += 1 - utilization[pm_index][0] / H[pm_index][0];
			wasted_ram_resources += 1 - utilization[pm_index][1] / H[pm_index][1];
			wasted_net_resources += 1 - utilization[pm_index][2] / H[pm_index][2];
		}
	}

	// If no pms working return 0
	if ( working_pms == 0 ) {
		return 0;
	}  

	// total wasted resources of all PMs / num of working PMs
	wasted_cpu_resources_ratio = wasted_cpu_resources / working_pms;
	wasted_ram_resources_ratio = wasted_ram_resources / working_pms;
	wasted_net_resources_ratio = wasted_net_resources / working_pms;

	wasted_resources_obj[0] = wasted_cpu_resources_ratio;
	wasted_resources_obj[1] = wasted_ram_resources_ratio;
	wasted_resources_obj[2] = wasted_net_resources_ratio;

	// sum the wasted resources ratio and divide with the number of resources cosidered (3 in this case)
	wasted_resources_ratio = ( wasted_cpu_resources_ratio * alpha + wasted_ram_resources_ratio * beta + wasted_net_resources_ratio * gamma ) / 3;

	return  wasted_resources_ratio;
}

/**
 * power_consumption: returns the power comsumption
 * parameter: utilization matrix
 * parameter: H matrix
 * parameter: number of physical machines
 * returns: power comsumption
 */
float power_consumption (float **utilization, int **H, int h_size, int *working_pms) {

	*working_pms = 0;

	/* iterate on physical machines */
	int iterator_physical;
	float utilidad = 0;
	float power_consumption = 0;

	for (iterator_physical = 0 ; iterator_physical < h_size ; iterator_physical++) {
		if (utilization[iterator_physical][0] > 0) {
			*working_pms = *working_pms + 1;
			/* calculates utility of a physical machine */
			utilidad = (float)utilization[iterator_physical][0] / (float)H[iterator_physical][0];
			/* calculates energy consumption of a physical machine */
			power_consumption += ((float)H[iterator_physical][3] - ((float)H[iterator_physical][3]*0.6)) * utilidad 				   
			+ (float)H[iterator_physical][3]*0.6;
		}
	}
	return power_consumption;
}

/**
 * calculates_weighted_sum: Calculates Weighted Sum of the Objetive Functions 
 *
 * parameter: power                  OF.1 Power Comsuption
 * parameter: total_revenue          OF.2 Economical Revenue	
 * parameter: wasted_resources_ratio OF.3 Wasted Resources Ratio
 * parameter: total_qos              OF.4 Quality of Service
 *
 * return 
 */
float calculates_weighted_sum(float power, double total_revenue, float wasted_resources_ratio, double total_qos) {

	float power_normalized = power * SIGMA_POWER;
	double revenue_normalized = total_revenue * SIGMA_REVENUE;
	float wasted_resources_normalized = wasted_resources_ratio * SIGMA_RESOURCES;
	double qos_normalized = total_qos * SIGMA_QOS;

	//printf("P: %f\t R: %f\t WR: %f\t QoS: %f \n", power_normalized, revenue_normalized, wasted_resources_normalized, qos_normalized );

	return power_normalized + revenue_normalized + wasted_resources_normalized + qos_normalized;

}

float calculate_average_from_array(float * array, int size) {
	int index;
	float sum = 0;

	for (index = 0; index < size; ++index) {
		sum += array[index];
	}

	return sum / size;
}