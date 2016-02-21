/* 
 * pareto.c: Virtual Machine Placement Problem - Pareto Functions
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
/* include arai headers */
#include "pareto.h"
#include "common.h"
#include "network.h"

/* structure of an element */
struct pareto_element
{
	int *solution;
	float *costs;
	struct pareto_element *prev;
	struct pareto_element *next;
};

struct pareto_element * pareto_create(int v_size, int *individual, float *objectives_functions)
{
	struct pareto_element *pareto_current = (struct pareto_element *) malloc (sizeof(struct pareto_element));
	pareto_current->solution = (int *) malloc (v_size *sizeof(int));
	pareto_current->costs = (float *) malloc (5 *sizeof(float));
	pareto_current->solution = individual;
	pareto_current->costs = objectives_functions;
	pareto_current->prev = NULL;
	pareto_current->next = NULL;
	return pareto_current;
}

struct pareto_element * pareto_insert(struct pareto_element *pareto_head, int v_size, int *individual, float *objectives_functions)
{
	struct pareto_element *pareto_current = pareto_create(v_size, individual, objectives_functions);
	struct pareto_element *ptr1;
	ptr1 = pareto_head;

	if (pareto_head == NULL)
	{
	    pareto_head = pareto_current;
		return pareto_head;
	}
	else
	{
		int iterator_virtual;
		int duplicate = 1;
		/* is it duplicate? */
		while(ptr1 != NULL)
		{
			for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
			{
		   		if(ptr1->solution[iterator_virtual] != pareto_current->solution[iterator_virtual])
		   		{
					duplicate = 0;
					break;
				}
			}
			if(duplicate == 1)
			{
				return pareto_head;
			}
			duplicate = 1;
			ptr1 = ptr1->next;
		}
		/* is it NOT duplicate */
		pareto_head->prev = pareto_current;
		pareto_current->next = pareto_head;
		pareto_head = pareto_current;
		return pareto_head;
	}
}

/* reference: http://www.geeksforgeeks.org/remove-duplicates-from-an-unsorted-linked-list/ */
void pareto_remove_duplicates(struct pareto_element *pareto_head, int v_size)
{
	struct pareto_element *ptr1, *ptr2, *dup;
	ptr1 = pareto_head;
	int iterator_virtual;
	int duplicate = 1;
	/* Pick elements one by one */
	while(ptr1 != NULL && ptr1->next != NULL)
	{
		ptr2 = ptr1;
		/* Compare the picked element with rest of the elements */
     	while(ptr2->next != NULL)
     	{
			for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
			{
		   		if(ptr1->solution[iterator_virtual] != ptr2->next->solution[iterator_virtual])
		   		{
					duplicate = 0;
					ptr2 = ptr2->next;
					break;
			   	}
			}
			if(duplicate == 1)
			{
			  	dup = ptr2->next;
			  	ptr2->next = ptr2->next->next;
			  	free(dup);
				printf("DUPLICATE!!!");
			}
			duplicate = 1;
		}
		ptr1 = ptr1->next;
	}
}

/* get_min_cost: calculate the min value of an objective function in a population
 * parameter: array of the values of the objective functions
 * parameter: objective functions id for evaluation
 * parameter: number of individuals
 * parameter: non-dominated sorting fronts
 * returns: min cost of the identified objetive function
 */
float get_min_cost(struct pareto_element *pareto_head, int objective)
{
	float min_cost = BIG_COST;
	struct pareto_element *ptr1;
	ptr1 = pareto_head;
	/* Pick elements one by one */
	while(ptr1 != NULL)
	{
		if(ptr1->costs[objective] < min_cost)
		{
			min_cost = ptr1->costs[objective];
		}
		ptr1 = ptr1->next;
	}
	return min_cost;
}

/* get_max_cost: calculate the max value of an objective function in a population
 * parameter: array of the values of the objective functions
 * parameter: objective functions id for evaluation
 * parameter: number of individuals
 * parameter: non-dominated sorting fronts
 * returns: max cost of the identified objetive function
 */
float get_max_cost(struct pareto_element *pareto_head, int objective)
{
	float max_cost = -1.0;
	struct pareto_element *ptr1;
	ptr1 = pareto_head;
	/* Pick elements one by one */
	while(ptr1 != NULL)
	{
		if(ptr1->costs[objective] > max_cost)
		{
			max_cost = ptr1->costs[objective];
		}
		ptr1 = ptr1->next;
	}
	return max_cost;
}

void print_pareto_set(struct pareto_element *pareto_head, int v_size)
{
	int iterator_virtual;
	while (pareto_head != NULL)
	{
		/* iterate on columns */
		for (iterator_virtual = 0; iterator_virtual < v_size; iterator_virtual++)
		{
			printf("%d\t",pareto_head->solution[iterator_virtual]);	
		}
		printf("\n");
		pareto_head = pareto_head->next;
	}
}

void print_pareto_front(struct pareto_element *pareto_head)
{
	while (pareto_head != NULL)
	{
		printf("%g\t%g\t%g\t%g\t%g\t",pareto_head->costs[0],pareto_head->costs[1],pareto_head->costs[2],pareto_head->costs[3],pareto_head->costs[4]);	
		printf("\n");
		pareto_head = pareto_head->next;
	}
}

int load_pareto_size(struct pareto_element *pareto_head)
{
	struct pareto_element *ptr1 = pareto_head;
	int size = 0;
	while(ptr1 != NULL)
	{
		size++;
		ptr1 = ptr1->next;
	}
	return size;
}

void report_best_population(struct pareto_element *pareto_head, int **G, int **T, int **H, int **V, int v_size, int h_size, int l_size)
{
	int pareto_size = load_pareto_size(pareto_head);
	int **best_P = (int **) malloc (pareto_size *sizeof (int *));
	float **objective_functions_best_P = (float **) malloc (pareto_size *sizeof (float *));
	FILE *pareto_set;
	FILE *pareto_front;
	FILE *pareto_network_utilization;
	FILE *pareto_cpu_utilization;
	FILE *pareto_ram_utilization;
	FILE *pareto_hdd_utilization;
	pareto_front = fopen("results/pareto_front","a");
	pareto_set = fopen("results/pareto_set","a");
	pareto_network_utilization = fopen("results/pareto_network_utilization","a");
	pareto_cpu_utilization = fopen("results/pareto_cpu_utilization","a");
	pareto_ram_utilization = fopen("results/pareto_ram_utilization","a");
	pareto_hdd_utilization = fopen("results/pareto_hdd_utilization","a");
	int iterator_individual;
	int iterator_virtual;
	int iterator_link;
	int iterator_physical;
	/* copying pareto individuals */	
	for (iterator_individual=0; iterator_individual < pareto_size; iterator_individual++)
	{
		best_P[iterator_individual] = (int *) malloc (v_size *sizeof (int));
		objective_functions_best_P[iterator_individual] = (float *) malloc (5 *sizeof (float));
		best_P[iterator_individual] = pareto_head->solution;
		objective_functions_best_P[iterator_individual] = pareto_head->costs;
		pareto_head = pareto_head->next;
	}
	/* once the best population is on a matrix, non-dominated sorting is performed */
	int *fronts_best_P = (int *) non_dominated_sorting(objective_functions_best_P, pareto_size);
	int ***utilization_best_P = load_utilization(best_P, H, V, pareto_size, h_size, v_size);
	int **network_utilization_best_P = load_network_utilization(best_P, G, T, pareto_size, l_size, v_size);
	/* save the Pareto set and the Pareto front */
	/* for each individual */
	for (iterator_individual=0; iterator_individual < pareto_size; iterator_individual++)
	{
		/* only the first pareto front */
		if(fronts_best_P[iterator_individual]==1)
		{
			/* Pareto set */
			for (iterator_virtual = 0; iterator_virtual < v_size ; iterator_virtual++)
			{
				/* for each element of the individual/solution */
				fprintf(pareto_set,"%d\t",best_P[iterator_individual][iterator_virtual]);
			}
			fprintf(pareto_set,"\n");

			/* Pareto front */
			fprintf(pareto_front,"%g\t%g\t%g\t%g\t%g\n",objective_functions_best_P[iterator_individual][0],objective_functions_best_P[iterator_individual][1], 
			objective_functions_best_P[iterator_individual][2],objective_functions_best_P[iterator_individual][3],objective_functions_best_P[iterator_individual][4]);

			/* network link utilization */
			for (iterator_link = 0; iterator_link < l_size ; iterator_link++)
			{
				/* for each element of the individual/solution */
				fprintf(pareto_network_utilization,"%d\t",network_utilization_best_P[iterator_individual][iterator_link]);
			}
			fprintf(pareto_network_utilization,"\n");

			/* physical resources utilization */
			for (iterator_physical = 0; iterator_physical < h_size ; iterator_physical++)
			{
				/* for each physical resource */
				fprintf(pareto_cpu_utilization,"%d\t",utilization_best_P[iterator_individual][iterator_physical][0]);
				fprintf(pareto_ram_utilization,"%d\t",utilization_best_P[iterator_individual][iterator_physical][1]);
				fprintf(pareto_hdd_utilization,"%d\t",utilization_best_P[iterator_individual][iterator_physical][2]);
			}
			fprintf(pareto_cpu_utilization,"\n");
			fprintf(pareto_ram_utilization,"\n");
			fprintf(pareto_hdd_utilization,"\n");
		}
	}
	fclose(pareto_set);
	fclose(pareto_front);
	return;
}
