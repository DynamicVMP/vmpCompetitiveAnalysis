/* 
 * imavmp.c: Interactive Memetic Algorithm for Virtual Machine Placement (IMAVMP)
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* structure of a pareto element */
struct pareto_element{
	int *solution;
	float *costs;
	struct pareto_element *prev;
	struct pareto_element *next;
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
		int v_size = get_v_size(argv[1]);
		int l_size = get_l_size(argv[1]);
		printf("\nH=%d, V=%d, L=%d\n",h_size,v_size,l_size);
		/* load physical machines resources, virtual machines requirements and network topology from the datacenter infrastructure file */
		int **H = load_H(h_size, argv[1]);
		// printf("\nH LOADED SUCCESSFULLY\n");
		int **V = load_V(v_size, argv[1]);	
		// printf("\nV LOADED SUCCESSFULLY\n");
		int **T = load_T(v_size, argv[1]);
		// printf("\nT LOADED SUCCESSFULLY\n");
		int **G = load_G(h_size, l_size, argv[1]);
		// printf("\nG LOADED SUCCESSFULLY\n");
		int *K 	= load_K(l_size, argv[1]);
		// printf("\nK LOADED SUCCESSFULLY\n");
		/* seed for rand() */
		// srand((unsigned int) time(NULL));
		/* randon value of 0-1 */
		// srand48(time(NULL));
		printf("\nDATACENTER LOADED SUCCESSFULLY\n");
		/* finish him */
		return 0;
	}
}

/* check_instance: checks if the problem instance has at least one solution
 * returns: 1 if there is no solution, 0 if the is at least one solution
 */
int check_instance()
{
	/* this return alllways 0 for now (this could be implemented this to check if a problem instance have at least one solution) */
	return 0;
}
