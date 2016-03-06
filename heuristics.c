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
	int iterator_scenario;

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
	int iterator_scenario;

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
 * prepare_input_for_decreasing_heuristics:
 * parameter S      [description]
 * parameter s_size [description]
 * return: nothing, it's a void	function
 */
void prepare_input_for_decreasing_heuristics(float **S, int s_size){
	int s_index = 0;
	float * temp_scenario = (float *) malloc (14 *sizeof (float));
	bool need_changes = true;

	// TODO: use a more efficient sorting algorithm
	while(need_changes) {
		need_changes = false;
		for (s_index = 0; s_index < s_size - 1; s_index++) {
			if (!is_better_than(S[s_index], S[s_index + 1])) {
				memcpy(temp_scenario, S[s_index], 14 * sizeof(float));
				memcpy(S[s_index], S[s_index + 1], 14 * sizeof(float));
				memcpy(S[s_index + 1], temp_scenario, 14 * sizeof(float));
				need_changes = true;
			}
		}
	}

	free(temp_scenario);
}

/**
 * is_better_than:
 *
 * parameter:
 * parameter:
 *
 * return 
 */
bool is_better_than(float* scenario_A, float* scenario_B){
	// If the scenario A occurs before B, A is better than B
	if(scenario_A[0] < scenario_B[0]){
		return true;
	}
	// TODO: define a DCR to use for the comparison
	// For now we compare the CPU requested for the VMs while the DCR (resource-Demand to server-Capacity
	// Ratio) is not defined yet.
	if(scenario_A[4] > scenario_B[4]){
		return true;
	}
	// B is better than A
	return false;
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
 * print_PM_list description: Clean memory
 * parameter: List of nodes
 * returns: nothing, it's a void function.
 */
void print_VM_list(VM_tend* list_to_free) {
	VM_tend* tmp_pointer;
	printf("\n");
	while(list_to_free != NULL){
		tmp_pointer = list_to_free;
		list_to_free = list_to_free->next;
		printf("(VM: %d, ", tmp_pointer->vm_index);
		printf("PM: %d, ", tmp_pointer->pm);
		printf("Ttend: %d)\t", tmp_pointer->tend);
		printf("->");
	}
	printf("NULL\n");
}

/**
 * print_PM_list description: Clean memory
 * parameter: List of nodes
 * returns: nothing, it's a void function.
 */
void print_PM_list(PM_weight_pair_node* list_to_free) {
	PM_weight_pair_node* tmp_pointer;
	printf("\n");
	while(list_to_free != NULL){
		tmp_pointer = list_to_free;
		list_to_free = list_to_free->next;
		printf("(PM: %d, ", tmp_pointer->h_index);
		printf("Weight: %d)\t", tmp_pointer->weight);
		printf("->");
	}
	printf("NULL\n");
}