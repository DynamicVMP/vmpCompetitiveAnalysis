/* 
 * imavmp.c: Interactive Memetic Algorithm for Virtual Machine Placement (IMAVMP)
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* Struct for PM Utilization */
struct pm_utilization{
	int totalRam;
	int totalCpu;
	int totalHdd;
};

struct physical_machine {
	int ram;
	int cpu;
	int hdd;
	int powerConsumption;
};

/* include libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
/* include own headers */
#include "common.h"	 	
#include "network.h"
#include "print_functions.h"
#include "heuristics.h"
#include "initialization.h"

/* definitions (this could be parameters) */
#define NUMBER_OF_INDIVIDUALS 100
#define NUMBER_OF_GENERATIONS 1
#define SELECTION_PERCENT 0.5
#define MAX_SLA 1

/* 
 * parameter: path to the datacenter infrastructure file
 * returns: exit state
 */
int main (int argc, char *argv[]) {
    
    int iterator_row;
	int iterator_column;

    /* parameters verification */
	if (argc == 1) {
		/* wrong parameters */	
		printf("[ERROR] usage: %s datacenter_file\n", argv[0]);
		/* finish him */
		return 1;
	} 
    /* good parameters */	
	else {
		/* get the number of physical machines, virtual machines and network links from the datacenter infrastructure file (argv[1]) */
		int h_size = get_h_size(argv[1]);
		int s_size = get_s_size(argv[1]);

		printf("\nH=%d, S=%d\n" ,h_size, s_size);
		/* load physical machines resources, virtual machines requirements and network topology from the datacenter infrastructure file */
		int **placement = (int **) malloc (h_size *sizeof (int *));
		float **utilization = heuristics_utilization_initialization(h_size);

		// print_float_matrix(utilization, h_size, 2);

		int **H = load_H(h_size, argv[1]);
		printf("\nPHYSICAL MACHINES LOADED SUCCESSFULLY\n");
		float **S = load_S(s_size, argv[1]);
		printf("\nSCENARIOS LOADED SUCCESSFULLY\n");

		printf("\nDATACENTER LOADED SUCCESSFULLY\n");
	
		for (iterator_row = 0; iterator_row < s_size; iterator_row++) {
			for (iterator_column = 0; iterator_column < 1; iterator_column++) {
				printf("Tiempo t: %g\n", S[iterator_row][iterator_column]);
				first_fit(S[iterator_row], utilization, placement, H, h_size);
			}
			printf("\n");
		}

	
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
