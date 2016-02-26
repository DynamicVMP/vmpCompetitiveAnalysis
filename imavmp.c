/* 
 * imavmp.c: Heuristic Algorithms for Virtual Machine Placement (IMAVMP)
 * Date: 17-11-2014
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 * Author: Rodrigo Ferreira (rodrigofepy@gmail.com)
 */

/* include own headers */
#include "heuristics.h"
#include "initialization.h"
#include "scenario.h"

/* definitions (this could be parameters) */
#define NUMBER_OF_SERVICES 1
#define NUMBER_OF_DATACENTER 1
#define NUMBER_VM_PER_DC 4 			// Number of VMs VMj in DCc;
#define RESOURCES 3 				
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
	int requestRejected = 0;

	FILE *power_consumption_file;
	power_consumption_file = fopen("results/power_consumption","a");

	int (*heuristics_array[3]) (float *S, float **utilization, int **placement, int **H, int h_size);
	char *heuristics_names[] = {"FIRST FIT", "BEST FIT", "WORST FIT"};

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

		printf("\nH=%d, S=%d\n", h_size, s_size);

		int **H = load_H(h_size, argv[1]);
		// printf("\nPHSYICAL MACHINES LOADED SUCCESSFULLY\n");
		float **S = load_S(s_size, argv[1]);
		// printf("\nSCENARIOS LOADED SUCCESSFULLY\n");
		
		// Placement matrix 
		int **placement = placement_initialization(h_size, NUMBER_VM_PER_DC);
		// print_int_matrix(placement, NUMBER_VM_PER_DC, h_size);
		
		// Utilization matrix 
		float **utilization = utilization_initialization(h_size, RESOURCES);
		// print_float_matrix(utilization, RESOURCES, h_size);
		
		printf("\nDATACENTER LOADED SUCCESSFULLY\n");
		printf("\n STARTING THE EXPERIMENT \n");
		printf("\n SELECT THE HEURISTIC TO USE");
		printf("\n1-) First Fit");
		printf("\n2-) Best Fit");
		printf("\n3-) Worst Fit");

		int heuristic = 0;
		while (heuristic == 0){
			printf("\n Option: ");
			scanf("%d",&heuristic);
			if(heuristic > 3 || heuristic < 1) {
				printf("\n INVALID OPTION, PLEASE SELECT THE HEURISTIC TO USE");
				heuristic = 0;
			}
		}

		printf("\n USING %s HEURISTIC", heuristics_names[heuristic-1]);
		int tiempo = 0;
		// printf("\nInstant t: 0");
		for (iterator_row = 0; iterator_row < s_size; ++iterator_row) {
 			if(S[iterator_row][0] != tiempo ) {
				fprintf(power_consumption_file, "%g\n", power_consumption(utilization, H, h_size));
				// printf("Instant t: %g", S[iterator_row][iterator_column]);
				tiempo = S[iterator_row][0];
			}
			(*heuristics_array[heuristic-1]) (S[iterator_row], utilization, placement, H, h_size);
		}
		float power = power_consumption(utilization, H, h_size);
		printf("\nPower Consumption(t= %d) :  %g\n", tiempo, power);
		fprintf(power_consumption_file, "%g\n", power);
		printf("\nFINAL - PLACEMENT\n");
		print_int_matrix(placement, NUMBER_VM_PER_DC, h_size);
		printf("\nFINAL - UTILIZATION\n");
		print_float_matrix(utilization, RESOURCES, h_size);

		/* CLEANING */
		free_float_matrix(utilization, h_size);
		free_int_matrix(placement, 3);
		free_int_matrix(H, h_size);
		free_float_matrix(S, s_size);

		printf("\n EXPERIMENT COMPLETED \n");
		
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
