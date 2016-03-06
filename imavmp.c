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

/* definitions (this could be parameters) */
#define NUMBER_OF_SERVICES 1
#define NUMBER_OF_DATACENTER 1
#define NUMBER_VM_PER_DC 300		// Number of VMs VMj in DCc;
#define RESOURCES 3
/*#define NUMBER_OF_DATACENTER 3*/
/*#define NUMBER_VM_PER_DC 4*/

#define MAX_SLA 4

/* 
 * parameter: path to the datacenter infrastructure file
 * returns: exit state
 */
int main (int argc, char *argv[]) {
    
    // Iterators 
    int iterator_row;
	int iterator_column;
	int iterator_physical;
	int iterator_service;
	int iterator_datacenter;
	int request_rejected = 0;
	int iterator_pm;

	// File pointers
	FILE *power_consumption_file;
	FILE *cpu_utilization;
	FILE *ram_utilization;
	FILE *net_utilization;
	FILE *execution_time_file;

	// Timer
	clock_t start;
	clock_t diff;
	int msec;

	// Files
	execution_time_file = fopen("results/time","a");
	power_consumption_file = fopen("results/power_consumption","a");
	cpu_utilization = fopen("results/cpu_utilization", "a");
	ram_utilization = fopen("results/ram_utilization", "a");
	net_utilization = fopen("results/net_utilization", "a");

	int (*heuristics_array[3]) (float *S, float **utilization, int **placement, int **H, int h_size, int *request_rejected, VM_tend** VM_list);

	char *heuristics_names[] = {"FIRST FIT", "BEST FIT", "WORST FIT", "FIRST FIT DECREASING", "BEST FIT DECREASING"};

	VM_tend* VM_list = (VM_tend*)calloc(1,sizeof(VM_tend));
	VM_list->vm_index = -1;
	VM_list->tend = 0;
	VM_list->pm = 0;
	VM_list->next = NULL;

	/*printf("Initial VM List");
	print_VM_list(VM_list);*/

	heuristics_array[0] = first_fit;
	heuristics_array[1] = best_fit;
	heuristics_array[2] = worst_fit;

    /* parameters verification */
	if (argc == 1) {
		/* wrong parameters */	
		printf("[ERROR] usage: %s datacenter_file\n", argv[0]);
		/* finish him */
		return 1;
	} 
    /* good parameters */	
	else {
		/* get the number of physical machines and virtual machines from the datacenter infrastructure file (argv[1]) */
		int h_size = get_h_size(argv[1]);
		int s_size = get_s_size(argv[1]);

		printf("\nTotal Physical Machines: %d\nTotal request: %d\n", h_size, s_size);

		int **H = load_H(h_size, argv[1]);
		// printf("\nPHSYICAL MACHINES LOADED SUCCESSFULLY\n");
		float **S = load_S(s_size, argv[1]);
		// printf("\nSCENARIOS LOADED SUCCESSFULLY\n");
		
		// Placement matrix 
		int **placement = placement_initialization(h_size, NUMBER_VM_PER_DC);
		// print_int_matrix(placement, NUMBER_VM_PER_DC, h_size);
		
		// Utilization matrix 
		float **utilization = utilization_initialization(h_size, RESOURCES);
		// print_float_matrix(utilization, h_size, RESOURCES);
		
		printf("\nDATACENTER LOADED SUCCESSFULLY\n");
		printf("\nSELECT THE HEURISTIC TO USE");
		printf("\n1-) First Fit");
		printf("\n2-) Best Fit");
		printf("\n3-) Worst Fit");
		printf("\n4-) First Fit Decreasing");
		printf("\n5-) Best Fit Decreasing");

		int heuristic = 0;
		while (heuristic == 0){
			printf("\n Option: ");
			scanf("%d",&heuristic);
			if(heuristic > 5 || heuristic < 1) {
				printf("\nINVALID OPTION, PLEASE SELECT THE HEURISTIC TO USE");
				heuristic = 0;
			}
		}

		printf("\nSTARTING THE EXPERIMENT");
		printf("\nUSING %s HEURISTIC", heuristics_names[heuristic-1]);
		int tiempo = 0;

		// Set Timer
		start = clock();
		if( heuristic > 3 && heuristic <= 5){
			prepare_input_for_decreasing_heuristics(S, s_size);
			heuristic -= 3;
		}

		for (iterator_row = 0; iterator_row < s_size; ++iterator_row) {
 			if(S[iterator_row][0] != tiempo ) {	

 				// Save FILE
				fprintf(power_consumption_file, "%g\n", power_consumption(utilization, H, h_size));
				print_placement_to_file("placement_result", placement, 4, h_size);
				print_utilization_matrix_to_file("utilization_result", utilization, h_size, RESOURCES);

				for (iterator_physical = 0; iterator_physical < h_size ; iterator_physical++) {
					/* for each physical resource */
					fprintf(cpu_utilization,"%g\t",utilization[iterator_physical][0]);
					fprintf(ram_utilization,"%g\t",utilization[iterator_physical][1]);
					fprintf(net_utilization,"%g\t",utilization[iterator_physical][2]);
				}
				fprintf(cpu_utilization,"\n");
				fprintf(ram_utilization,"\n");
				fprintf(net_utilization,"\n");

				// check_VM_tend_list and update the VM placement
				remove_VM_from_placement(VM_list, placement, utilization, tiempo, h_size);

				tiempo = S[iterator_row][0];
			}

			if(S[iterator_row][0] <= S[iterator_row][12]) {
				(*heuristics_array[heuristic-1]) (S[iterator_row], utilization, placement, H, h_size, &request_rejected, &VM_list);	
			}
		}
		float power = power_consumption(utilization, H, h_size);
		printf("\nPower Consumption(t= %d) :  %g\n", tiempo, power);
		
		diff = clock() - start;
		msec = diff * 1000 / CLOCKS_PER_SEC;

		// Save to FILE
		fprintf(power_consumption_file, "%g\n", power);
		print_placement_to_file("placement_result", placement, NUMBER_VM_PER_DC, h_size);
		print_utilization_matrix_to_file("utilization_result", utilization, h_size, RESOURCES);

		for (iterator_physical = 0; iterator_physical < h_size ; iterator_physical++) {
			/* for each physical resource */
			fprintf(cpu_utilization,"%g\t",utilization[iterator_physical][0]);
			fprintf(ram_utilization,"%g\t",utilization[iterator_physical][1]);
			fprintf(net_utilization,"%g\t",utilization[iterator_physical][2]);
		}

		// RESULTS
		/* printf("\nFINAL - PLACEMENT\n");
		print_int_matrix(placement, NUMBER_VM_PER_DC, h_size);
		printf("\nFINAL - UTILIZATION\n");
		print_float_matrix(utilization, h_size, RESOURCES);*/
		printf("\nRESULTS\n");
		printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
		printf("Number of times the objective function was assessed: %d\n", tiempo);
		printf("Number of rejected requests: %d\n" , request_rejected);
		fprintf(execution_time_file, "%d:%d\n", msec/1000, msec%1000);

		/* CLEANING */
		free_float_matrix(utilization, h_size);
		free_int_matrix(placement, 3);
		free_int_matrix(H, h_size);
		free_float_matrix(S, s_size);
		free_VM_list(VM_list);

		printf("\nEXPERIMENT COMPLETED\n");
		
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
