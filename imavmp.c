/* 
 * imavmp.c: Interactive Memetic Algorithm for Virtual Machine Placement (IMAVMP)
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include own headers */
#include "heuristics.h"
#include "initialization.h"
#include "scenario.h"

/* definitions (this could be parameters) */
#define NUMBER_OF_SERVICES 1
#define NUMBER_OF_DATACENTER 1
#define NUMBER_VM_PER_DC 100 // Number of VMs VMj in DCc;
#define RESOURCES 3 // Number of VMs VMj in DCc;
#define MAX_SLA 1

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
		
		// Utilization matrix 
		float **utilization = utilization_initialization(h_size, RESOURCES);
		
		/* int ****placement = (int ****) malloc (h_size *sizeof (int ***));
		for(iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
			placement[iterator_physical] = (int ***) malloc (sizeof (int **));
			for (iterator_service = 0; iterator_service < NUMBER_OF_SERVICES; iterator_service++) {
				placement[iterator_physical][iterator_service] = (int **) malloc (sizeof (int *));
				for(iterator_datacenter = 0; iterator_datacenter < NUMBER_OF_DATACENTER; iterator_datacenter++) {
					placement[iterator_physical][iterator_service][iterator_datacenter] = (int *) malloc (sizeof (int));		
				}	
			}
		}*/

		// printf("\nUTILIZATION\n");
		// print_float_matrix(utilization, h_size, 3);
		
		// printf("\nPLACEMENT\n");
		// print_int_matrix(placement, 3, h_size);
		printf("\nDATACENTER LOADED SUCCESSFULLY\n");
		printf("\n STARTING THE EXPERIMENT \n");

		int tiempo = 0;
		for (iterator_row = 0; iterator_row < s_size; ++iterator_row) {
			if(S[iterator_row][0] != tiempo ) {
				printf("\nPower Consumption(t= %d) :  %g\n\n", tiempo, power_consumption(utilization, H, h_size));
				// printf("\nPLACEMENT\n");
				// print_int_matrix(placement, 3, h_size);
				tiempo = S[iterator_row][0];
			}
			printf("Tiempo t: %g", S[iterator_row][iterator_column]);
			first_fit(S[iterator_row], utilization, placement, H, h_size);
			// best_fit(S[iterator_row], utilization, placement, H, h_size);
			// worst_fit(S[iterator_row], utilization, placement, H, h_size);
		}
		printf("\nPower Consumption(t= %d) :  %g\n", tiempo, power_consumption(utilization, H, h_size));
		printf("\nFINAL - PLACEMENT\n");
		print_int_matrix(placement, 3, h_size);


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
