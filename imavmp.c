/* 
 * imavmp.c: Heuristic Algorithms for Virtual Machine Placement (IMAVMP)
 * Date: February, 2016
 * Author: Rodrigo Ferreira (rodrigofepy@gmail.com)
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */

/* include own headers */
#include "heuristics.h"
#include "initialization.h"
#include "scenario.h"

/* definitions */
#define RESOURCES 3
#define VM_FEATURES 11
#define MAX_SLA 4

/* 
 * parameter 1: path to the datacenter infrastructure file
 * parameter 2(optional): selected heuristic
 * returns: exit state
 */
int main (int argc, char *argv[]) {
    
    // Iterators 
    int iterator_row;
	int iterator_physical;
	int i;

	// Variables
	int request_serviced = 0;
	int request_update = 0;
	int request_rejected = 0;
	int total_t = 0;
	int index = 0;
	int working_pms = 0;
	int living_vms = 0;
	int living_derived_vms = 0;
	long revenue_a_priori = 0;
	long qos_a_priori = 0;
	int vm_migrated = 0;
	int heuristic = 0;
	int time_unit;
	double * total_revenue_array;
	long * total_qos_array;
	float * wasted_resources_ratio_array;
	float ** wasted_resources_array;
	float * power_consumption_array;
	long revenue_a_priori_t = 0;
	long revenue_t = 0;
	long total_revenue = 0;
	long total_qos = 0;
	long remove_revenue;


	bool first_time = true;
	char file_postfix [300] = "";
	char file_name [300] = "";
	char * temp_file_name;
	char aux_file_name [300] = "";


	double max_revenue = 0;
	double min_revenue; 
	long max_qos = 0; 
	long min_qos;
	float max_wasted_resources, min_wasted_resources;
	float max_power, min_power;

	// File pointers
	FILE *power_consumption_file;
	FILE *economical_revenue_file;
	FILE *quality_service_file;
	FILE *wasted_resources_file;
	FILE *execution_time_file;
	FILE *weighted_sum_file;
	FILE *pm_usage;
	FILE *utilization_result;

	// Heuristic Names
	char *heuristics_names[] = {"FIRST FIT", "BEST FIT", "WORST FIT", "FIRST FIT DECREASING", "BEST FIT DECREASING"};

	// Timer
	clock_t start;
	clock_t diff;
	int msec;

	if ( argc > 2 ) {
		strcpy(aux_file_name, argv[1]);
		temp_file_name = strtok (aux_file_name, "/");
		while (temp_file_name != NULL)
		{
			sprintf(file_name,"%s",temp_file_name);
			temp_file_name = strtok (NULL, "/");
		}
		sprintf(file_postfix,"-%s-%s",file_name,heuristics_names[atoi(argv[2])-1]);
	}
	// Files
	sprintf(file_name,"results/time%s",file_postfix);
	execution_time_file = fopen(file_name,"a");

	// Objective functions files
	sprintf(file_name,"results/power_consumption%s",file_postfix);
	power_consumption_file = fopen(file_name,"a");

	sprintf(file_name,"results/economical_revenue%s",file_postfix);
	economical_revenue_file = fopen(file_name,"a");

	sprintf(file_name,"results/quality_service%s",file_postfix);
	quality_service_file = fopen(file_name,"a");

	sprintf(file_name,"results/wasted_resources%s",file_postfix);
	wasted_resources_file = fopen(file_name,"a");

	sprintf(file_name,"results/weighted_sum%s",file_postfix);
	weighted_sum_file = fopen(file_name,"a");

	sprintf(file_name,"results/pm_usage%s",file_postfix);
	pm_usage = fopen(file_name,"a");

	// Heuristic
	bool (*heuristics_array[3]) (float *S, float **utilization, float **resources_requested, int **placement, int **H, int h_size, VM_linked_list** VM_list_derived, VM_linked_list** VM_list, VM_linked_list** VM_list_serviced, VM_linked_list** VM_list_serviced_derived);

	// VM linked list wich contains all VM currently hosted by local DC
	VM_linked_list* VM_list = (VM_linked_list*)calloc(1,sizeof(VM_linked_list));
	VM_list->vm_index = -1;
	VM_list->tend = -1;
	VM_list->pm = -1;
	VM_list->SLA = 0;
	VM_list->revenue = 0;
	VM_list->next = NULL;

	VM_linked_list* VM_list_derived = (VM_linked_list*)calloc(1,sizeof(VM_linked_list));
	VM_list_derived->vm_index = -1;
	VM_list_derived->tend = -1;
	VM_list_derived->pm = -1;
	VM_list_derived->SLA = 0;
	VM_list_derived->revenue = 0;
	VM_list_derived->next = NULL;

	VM_linked_list* VM_list_serviced = (VM_linked_list*)calloc(1,sizeof(VM_linked_list));
	VM_list_serviced->vm_index = -1;
	VM_list_serviced->tend = -1;
	VM_list_serviced->pm = -1;
	VM_list_serviced->SLA = 0;
	VM_list_serviced->revenue = 0;
	VM_list_serviced->next = NULL;

	VM_linked_list* VM_list_serviced_derived = (VM_linked_list*)calloc(1,sizeof(VM_linked_list));
	VM_list_serviced_derived->vm_index = -1;
	VM_list_serviced_derived->tend = -1;
	VM_list_serviced_derived->pm = -1;
	VM_list_serviced_derived->SLA = 0;
	VM_list_serviced_derived->revenue = 0;
	VM_list_serviced_derived->next = NULL;

	heuristics_array[0] = first_fit;
	heuristics_array[1] = best_fit;
	heuristics_array[2] = worst_fit;

    /* Parameters verification */
	if (argc == 1) {
		/* wrong parameters */	
		printf("[ERROR] usage: %s datacenter_file\n", argv[0]);
		/* finish him */
		return 1;
	} 
    /* Good parameters */	
	else {

		/* Get the number of physical machines and requests from the datacenter infrastructure file (argv[1]) */
		int h_size = get_h_size(argv[1]);					// Number of Physical Machines
		int s_size = get_s_size(argv[1]);					// Number of requests
		int **H = load_H(h_size, argv[1]); 					// Load Physical Machines
		float **S = load_S(s_size, argv[1]);				// Load Scenario
		int unique_vms = number_unique_vm (S,s_size, &revenue_a_priori, &qos_a_priori);		// Number of Unique VM

		total_t = S[s_size - 1][0];

		total_revenue_array = (double *) calloc (total_t + 1, sizeof(double *));
		total_qos_array = (long *) calloc (total_t + 1, sizeof(long *));
		wasted_resources_ratio_array = (float *) calloc (total_t + 1, sizeof(float *));
		power_consumption_array = (float *) calloc (total_t + 1, sizeof(float *));

		wasted_resources_array = (float **) calloc (total_t + 1, sizeof(float *));
		for (i = 0; i < total_t + 1; i++) {
			wasted_resources_array[i] = (float *) calloc (3, sizeof(float)); 	//[0] = cpu, [1] = ram, [2] = net
		}

		// Initial Placement matrix
		int **placement = placement_initialization(VM_FEATURES, unique_vms);

		// Initial Utilization matrix
		float **utilization = utilization_initialization(h_size, RESOURCES);
		float **resources_requested = utilization_initialization(h_size, RESOURCES);

		if(argc < 3 ) {

			printf("\nVMP - HEURISTIC APPROACH \n\n");
			printf("Total Physical Machines: %d\n", h_size);
			printf("Total request: %d\n", s_size);
			printf("Unique VM: %d\n", unique_vms);
			printf("Economical Revenue a priori: %li\n", revenue_a_priori);
			printf("Quality of Service a priori: %li\n", qos_a_priori);

			printf("\nDATACENTER LOADED SUCCESSFULLY\n");
			printf("\nSELECT THE HEURISTIC TO USE");
			printf("\n1-) First Fit");
			printf("\n2-) Best Fit");
			printf("\n3-) Worst Fit");
			printf("\n4-) First Fit Decreasing");
			printf("\n5-) Best Fit Decreasing");

			while (heuristic == 0) {
				printf("\nOption: ");
				scanf("%d", &heuristic);
				if (heuristic > 5 || heuristic < 1) {
					printf("\nINVALID OPTION, PLEASE SELECT THE HEURISTIC TO USE");
					heuristic = 0;
				}
			}

			printf("\nSTARTING THE EXPERIMENT");
			printf("\nUSING %s HEURISTIC\n", heuristics_names[heuristic - 1]);
		} else {
			heuristic = atoi(argv[2]);
		}

		// Set Timer
		start = clock();
		if( heuristic > 3 && heuristic <= 5){
			prepare_input_for_decreasing_heuristics(S, s_size);
			heuristic -= 3;
		}

		time_unit = S[0][0];
		int initial_time_unit = time_unit;
		int time_unit_from_zero;
		for (iterator_row = 0; iterator_row < s_size; ++iterator_row) {
			revenue_a_priori_t = revenue_a_priori_t + S[iterator_row][10];
			// If current_time is equal to VM tinit, allocated VM  
			if(S[iterator_row][0] <= S[iterator_row][12]) {
				if( (*heuristics_array[heuristic-1]) (S[iterator_row], utilization, resources_requested, placement, H, h_size, &VM_list_derived, &VM_list, &VM_list_serviced, &VM_list_serviced_derived) ) {
					request_serviced++;
				} else {
					request_rejected++;
				}
			} else {
				// Update VM resources
				if(S[iterator_row][0] <= S[iterator_row][13] 
					&& update_VM_resources(placement, utilization, resources_requested, S[iterator_row], &VM_list, H)) {
					request_update++;
				} else {
					request_rejected++;
				}
			}

 			if(iterator_row + 1 == s_size || S[iterator_row][0] != S[iterator_row+1][0]) {
				time_unit_from_zero = time_unit - initial_time_unit;
 				// Calculates Objective Functions
 				remove_revenue = remove_VM_by_time(&VM_list, &VM_list_derived, placement, utilization, resources_requested, time_unit, h_size);
				economical_revenue(&VM_list, &VM_list_derived, &revenue_t, &total_qos_array[time_unit_from_zero], &living_vms, &living_derived_vms);
				power_consumption_array[time_unit_from_zero] = power_consumption(utilization, H, h_size, &working_pms);
				wasted_resources_ratio_array[time_unit_from_zero] = wasted_resources(utilization, resources_requested, H, h_size, wasted_resources_array[time_unit_from_zero]);

 				total_revenue_array[time_unit_from_zero] = revenue_a_priori_t - revenue_t;
				revenue_a_priori_t = revenue_a_priori_t - remove_revenue;

				// Calculates the max and min of each objective function
				if( first_time ) {
					min_power = power_consumption_array[time_unit_from_zero];
					max_power = power_consumption_array[time_unit_from_zero];
					min_qos = total_qos_array[time_unit_from_zero];
					max_qos = total_qos_array[time_unit_from_zero];
					min_revenue = total_revenue_array[time_unit_from_zero];
					max_revenue = total_revenue_array[time_unit_from_zero];
					min_wasted_resources = wasted_resources_ratio_array[time_unit_from_zero];
					max_wasted_resources = wasted_resources_ratio_array[time_unit_from_zero];
					first_time = false;
				} else {

					if ( power_consumption_array[time_unit_from_zero] < min_power ) {
						min_power = power_consumption_array[time_unit_from_zero];
					} else if ( power_consumption_array[time_unit_from_zero] > max_power ) {
						max_power = power_consumption_array[time_unit_from_zero];
					}

					if ( total_qos_array[time_unit_from_zero] < min_qos ) {
						min_qos = total_qos_array[time_unit_from_zero];
					} else if ( total_qos_array[time_unit_from_zero] > max_qos ) {
						max_qos = total_qos_array[time_unit_from_zero];
					}

					if ( total_revenue_array[time_unit_from_zero] < min_revenue ) {
						min_revenue = total_revenue_array[time_unit_from_zero];
					} else if ( total_revenue_array[time_unit_from_zero] > max_revenue ) {
						max_revenue = total_revenue_array[time_unit_from_zero];
					}

					if ( wasted_resources_ratio_array[time_unit_from_zero] < min_wasted_resources ) {
						min_wasted_resources = wasted_resources_ratio_array[time_unit_from_zero];
					} else if ( wasted_resources_ratio_array[time_unit_from_zero] > max_wasted_resources ) {
						max_wasted_resources = wasted_resources_ratio_array[time_unit_from_zero];
					}

				}
 				// Save to FILE
				fprintf(power_consumption_file, "%f\n", power_consumption_array[time_unit_from_zero] );
				fprintf(wasted_resources_file, "%f\n", wasted_resources_ratio_array[time_unit_from_zero]);
				fprintf(economical_revenue_file, "%f\n", total_revenue_array[time_unit_from_zero]);
				fprintf(quality_service_file, "%li\n", total_qos_array[time_unit_from_zero]);
				fprintf(pm_usage, "%d %d %d\n", working_pms, living_vms, living_derived_vms);
				print_utilization_matrix_to_file("utilization_result", utilization, h_size, RESOURCES);
				total_revenue += revenue_t;
				if ( iterator_row+1 != s_size ){
					time_unit = S[iterator_row+1][0];
				}
			}
		}


		float average_wasted_resource_ratio = calculate_average_from_array( wasted_resources_ratio_array, time_unit_from_zero + 1 );
		float average_power_consumption = calculate_average_from_array( power_consumption_array, time_unit_from_zero + 1);

		float normalized_wasted_resources_ratio = 0;
		float normalized_power_consumption = 0;

		economical_revenue(&VM_list_serviced, &VM_list_serviced_derived, &revenue_t, &total_qos, &living_vms, &living_derived_vms);

		// Normalized Revenue
		double normalized_revenue = 0;
		long delta_revenue = revenue_a_priori - total_revenue;
		double revenue_to_normalized = (double) delta_revenue;

		// Normalized QoS
		double normalized_qos = 0;
		long delta_qos = qos_a_priori - total_qos;
		double qos_to_normalized = (double) delta_qos;

		for(index = 0; index <= time_unit_from_zero; index++){
			
			normalized_power_consumption = (power_consumption_array[index] - min_power) / (max_power - min_power);
			
			if(total_revenue_array[index] > 0) {
				normalized_revenue =  ((double)total_revenue_array[index] - min_revenue) / (max_revenue - min_revenue);
			}else{
				normalized_revenue = 0;
			}

			normalized_wasted_resources_ratio = (wasted_resources_ratio_array[index] - min_wasted_resources) / (max_wasted_resources - min_wasted_resources);

			if(total_qos_array[index] > 0) {
				normalized_qos = ((double)total_qos_array[index] - min_qos) / (max_qos - min_qos);
			}else{
				normalized_qos = 0;
			}

			fprintf(weighted_sum_file, "%f\n", calculates_weighted_sum(normalized_power_consumption, normalized_revenue, normalized_wasted_resources_ratio, normalized_qos));
		}


		normalized_power_consumption = (average_power_consumption - min_power) / (max_power - min_power);
		if(delta_revenue > 0) {
			normalized_revenue =  (revenue_to_normalized - min_revenue) / (max_revenue - min_revenue);
		}else{
			normalized_revenue = 0;
		}
		normalized_wasted_resources_ratio = (average_wasted_resource_ratio - min_wasted_resources) / (max_wasted_resources - min_wasted_resources);
		if(delta_qos > 0) {
			normalized_qos = (qos_to_normalized - min_qos) / (max_qos - min_qos);
		}else{
			normalized_qos = 0;
		}

		float weighted_sum = calculates_weighted_sum(normalized_power_consumption, normalized_revenue, normalized_wasted_resources_ratio, normalized_qos);

		fprintf(weighted_sum_file, "%f\n", weighted_sum);
		diff = clock() - start;
		msec = diff * 1000 / CLOCKS_PER_SEC;

		fprintf(execution_time_file, "%d:%d\n", msec/1000, msec%1000);

		if(argc < 3 ) {
			printf("\n************************RESULTS*************************\n");
			printf("Simulated time: %d time units.\n", time_unit);
			printf("Power Consumption: %.6g\n", average_power_consumption);
			printf("Economical Revenue a priori: %li\n", revenue_a_priori);
			printf("Economical Revenue: %li\n", total_revenue);
			printf("Delta - Economical Revenue: %li\n", delta_revenue);
			printf("Quality of Service a priori: %li\n", qos_a_priori);
			printf("Quality of Service: %li\n", total_qos);
			printf("Delta - Quality of Service: %li\n", delta_qos);
			printf("Wasted Resources: %.6g\n", average_wasted_resource_ratio);
			printf("Weighted Sum: %.8g\n", weighted_sum);
			printf("Time taken %d seconds %d milliseconds\n", msec / 1000, msec % 1000);
			printf("Number of times the objective function was assessed: %d\n", time_unit);
			printf("Number of updated requests succesful: %d\n", request_update);
			printf("Number of serviced requests succesful: %d\n", request_serviced);
			printf("Number of rejected requests succesful: %d\n", request_rejected);
			printf("********************************************************\n");
		}
		/* CLEANING */
		free_float_matrix(utilization, h_size);
		free_float_matrix(S, s_size);
		free_VM_list(VM_list);
		free_VM_list(VM_list_derived);
		free_VM_list(VM_list_serviced);
		free_VM_list(VM_list_serviced_derived);
		/* finish him */
		return 0;
	}
}

