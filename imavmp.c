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
 * parameter: path to the datacenter infrastructure file
 * returns: exit state
 */
int main (int argc, char *argv[]) {
    
    // Iterators 
    int iterator_row;
	int iterator_physical;

	// Variables
	int request_rejected = 0;
	float revenue_a_priori = 0;
	float qos_a_priori = 0;
	int vm_migrated = 0;
	int request_update = 0;
	int heuristic = 0;
	int time_unit = 0;
	float total_revenue = 0;
	double total_qos = 0;
	float wasted_resources_ratio = 0;
	float total_power = 0.0;

	// File pointers
	FILE *power_consumption_file;
	FILE *economical_revenue_file;
	FILE *quality_service_file;
	FILE *wasted_resources_file;
	FILE *cpu_utilization;
	FILE *ram_utilization;
	FILE *net_utilization;
	FILE *execution_time_file;
	FILE *weighted_sum_file;

	// Timer
	clock_t start;
	clock_t diff;
	int msec;

	// Files
	execution_time_file = fopen("results/time","a");

	// Objective functions files
	power_consumption_file = fopen("results/power_consumption","a");
	economical_revenue_file = fopen("results/economical_revenue","a");
	quality_service_file = fopen("results/quality_service","a");
	wasted_resources_file = fopen("results/wasted_resources","a");
	weighted_sum_file = fopen("results/weighted_sum","a");

	// Resources files
	cpu_utilization = fopen("results/cpu_utilization", "a");
	ram_utilization = fopen("results/ram_utilization", "a");
	net_utilization = fopen("results/net_utilization", "a");

	// Heuristic
	bool (*heuristics_array[3]) (float *S, float **utilization, float **resources_requested, int **placement, int **H, int h_size, int *request_rejected, VM_linked_list** VM_list);

	// Heuristic Names
	char *heuristics_names[] = {"FIRST FIT", "BEST FIT", "WORST FIT", "FIRST FIT DECREASING", "BEST FIT DECREASING"};

	VM_linked_list* VM_list = (VM_linked_list*)calloc(1,sizeof(VM_linked_list));
	VM_list->vm_index = -1;
	VM_list->tend = -1;
	VM_list->pm = -1;
	VM_list->next = NULL;

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
		printf("\nVMP - HEURISTIC APPROACH \n\n");

		/* Get the number of physical machines and requests from the datacenter infrastructure file (argv[1]) */
		int h_size = get_h_size(argv[1]);					// Number of Physical Machines
		int s_size = get_s_size(argv[1]);					// Number of requests
		int **H = load_H(h_size, argv[1]); 					// Load Physical Machines
		float **S = load_S(s_size, argv[1]);				// Load Scenario
		int unique_vms = number_unique_vm (S,s_size, &revenue_a_priori, &qos_a_priori);		// Number of Unique VM 
		
		printf("Total Physical Machines: %d\n", h_size); 
		printf("Total request: %d\n", s_size);
		printf("Unique VM: %d\n", unique_vms);
		printf("Economical Revenue a priori: %.6g\n", revenue_a_priori);
		printf("Quality of Service a priori: %.6g\n", qos_a_priori);

		// Initial Placement matrix 
		int **placement = placement_initialization(VM_FEATURES, unique_vms);
		
		// Initial Utilization matrix 
		float **utilization = utilization_initialization(h_size, RESOURCES);
		float **resources_requested = utilization_initialization(h_size, RESOURCES);
		
		printf("\nDATACENTER LOADED SUCCESSFULLY\n");
		printf("\nSELECT THE HEURISTIC TO USE");
		printf("\n1-) First Fit");
		printf("\n2-) Best Fit");
		printf("\n3-) Worst Fit");
		printf("\n4-) First Fit Decreasing");
		printf("\n5-) Best Fit Decreasing");

		while (heuristic == 0){
			printf("\nOption: ");
			scanf("%d",&heuristic);
			if(heuristic > 5 || heuristic < 1) {
				printf("\nINVALID OPTION, PLEASE SELECT THE HEURISTIC TO USE");
				heuristic = 0;
			}
		}

		printf("\nSTARTING THE EXPERIMENT");
		printf("\nUSING %s HEURISTIC\n", heuristics_names[heuristic-1]);
		
		// Set Timer
		start = clock();
		if( heuristic > 3 && heuristic <= 5){
			prepare_input_for_decreasing_heuristics(S, s_size);
			heuristic -= 3;
		}

		for (iterator_row = 0; iterator_row < s_size; ++iterator_row) {
 			
 			if(S[iterator_row][0] != time_unit ) {
				 // check_VM_linked_list and update the VM placement and utilization matrix
				remove_VM_by_time(&VM_list, placement, utilization, time_unit, h_size);
 				
 				// Calculates Objective Functions
				total_power = power_consumption(utilization, H, h_size);
 				wasted_resources_ratio = wasted_resources(utilization, resources_requested, H, h_size);
 				economical_revenue(&VM_list, &total_revenue, &total_qos);

 				// Save to FILE 
 				fprintf(power_consumption_file, "%.4g\n", total_power );
				fprintf(wasted_resources_file, "%.4g\n", wasted_resources_ratio);
				fprintf(economical_revenue_file, "%.4g\n", total_revenue);
				fprintf(quality_service_file, "%.4g\n", total_qos);
				fprintf(weighted_sum_file, "%.4g\n", calculates_weighted_sum(total_power, total_revenue, wasted_resources_ratio, total_qos ));
				print_placement_to_file("placement_result", placement, VM_FEATURES, unique_vms);
				print_utilization_matrix_to_file("utilization_result", utilization, h_size, RESOURCES);

				for (iterator_physical = 0; iterator_physical < h_size ; iterator_physical++) {
					/* for each physical resource */
					fprintf(cpu_utilization,"%.4g\t",utilization[iterator_physical][0]);
					fprintf(ram_utilization,"%.4g\t",utilization[iterator_physical][1]);
					fprintf(net_utilization,"%.4g\t",utilization[iterator_physical][2]);
				}
				fprintf(cpu_utilization,"\n");
				fprintf(ram_utilization,"\n");
				fprintf(net_utilization,"\n");

				time_unit = S[iterator_row][0];
			}

			// If current_time is equal to VM tinit, allocated VM  
			if(S[iterator_row][0] <= S[iterator_row][12]) {
				// printf("\nRequest: %g %g %g %g %g", S[iterator_row][0], S[iterator_row][1], S[iterator_row][2], S[iterator_row][3], S[iterator_row][13] );
				(*heuristics_array[heuristic-1]) (S[iterator_row], utilization, resources_requested, placement, H, h_size, &request_rejected, &VM_list);
			} else {
				// Update VM resources
				// printf("\nRequest: %g %g %g %g %g", S[iterator_row][0], S[iterator_row][1], S[iterator_row][2], S[iterator_row][3], S[iterator_row][13] );
				//  If current_time is less than VM tend
				if(S[iterator_row][0] <= S[iterator_row][13] 
					&& update_VM_resources(placement, utilization, resources_requested, S[iterator_row], &VM_list, H)) {
					// printf("\nVM Update successful!\n");
					request_update++;
				} else {
					// printf("\nCan not update the VM. Request rejected\n");
					request_rejected++;
					
					// For migration
					/*if ( (*heuristics_array[heuristic-1]) (S[iterator_row], utilization, resources_requested, placement, H, h_size, &request_rejected, &VM_list) ) {
						vm_migrated++;
						printf("\nVM migration successful\n");

					} else {
						printf("\nCan not migrate the VM. Request rejected\n");
					}*/
				}
			}
		}
		// Remove all VM
		remove_VM_by_time(&VM_list, placement, utilization, time_unit, h_size);
		
		// Calculates objective functions
		float power = power_consumption(utilization, H, h_size);
		economical_revenue(&VM_list, &total_revenue, &total_qos);
		wasted_resources_ratio = wasted_resources(utilization, resources_requested, H, h_size);

		double weighted_sum = calculates_weighted_sum(power, total_revenue, wasted_resources_ratio, total_qos);

		diff = clock() - start;
		msec = diff * 1000 / CLOCKS_PER_SEC;

		// Save to FILE
		fprintf(power_consumption_file, "%.4g\n", power);
		fprintf(economical_revenue_file, "%.4g\n", total_revenue);
		fprintf(wasted_resources_file, "%.4g\n", wasted_resources_ratio);
		fprintf(quality_service_file, "%.4g\n", total_qos);
		fprintf(weighted_sum_file, "%.4g\n", weighted_sum);

		print_placement_to_file("placement_result", placement, VM_FEATURES, unique_vms);
		print_utilization_matrix_to_file("utilization_result", utilization, h_size, RESOURCES);
		fprintf(execution_time_file, "%d:%d\n", msec/1000, msec%1000);

		for (iterator_physical = 0; iterator_physical < h_size ; iterator_physical++) {
			/* for each physical resource */
			fprintf(cpu_utilization,"%.4g\t",utilization[iterator_physical][0]);
			fprintf(ram_utilization,"%.4g\t",utilization[iterator_physical][1]);
			fprintf(net_utilization,"%.4g\t",utilization[iterator_physical][2]);
		}

		// RESULTS
		/*printf("\nFINAL - PLACEMENT\n");
		print_int_matrix(placement, VM_FEATURES, unique_vms);
		printf("\nFINAL - UTILIZATION\n");
		print_float_matrix(utilization, h_size, RESOURCES);
		printf("\nEXPERIMENT COMPLETED\n");*/
		printf("\n************************RESULTS*************************\n");
		printf("Simulated time: %d time units.\n", time_unit);
		printf("Power Consumption: %.4g\n", power);
		printf("Economical Revenue: %.4g\n", total_revenue);
		printf("Quality of Service: %.4g\n", total_qos);
		printf("Wasted Resources: %.4g\n", wasted_resources_ratio);
		printf("Weighted Sum: %.8g\n", weighted_sum);
		printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
		printf("Number of times the objective function was assessed: %d\n", time_unit);
		printf("Number of update requests succesful: %d\n" , request_update);
		printf("********************************************************\n");

		/* CLEANING */
		free_float_matrix(utilization, h_size);
		free_int_matrix(placement, 9);
		free_int_matrix(H, h_size);
		free_float_matrix(S, s_size);
		// print_VM_list(VM_list);
		free_VM_list(VM_list);

		/* finish him */
		return 0;
	}
}

/* check_instance: checks if the problem instance has at least one solution
 * returns: 1 if there is no solution, 0 if the is at least one solution
 */
int check_instance() {
	/* this return alllways 0 for now (this could be implemented this to check if a problem instance have at least one solution) */
	return 0;
}
