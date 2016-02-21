/* 
 * initialization.c: Virtual Machine Placement Problem - Initialization Stage
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include initialization stage header */
#include "initialization.h"

/* initialization: initializes a population randomically
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * parameter: virtual machines requirements matrix
 * returns: population matrix
 */
int** initialization(int number_of_individuals, int h_size, int v_size, int **V, int max_SLA)
{
	/* population: initial population matrix */
	int **population = (int **) malloc (number_of_individuals *sizeof (int *));
	/* iterators */
	int iterator_individual;
	int iterator_individual_position;
	/* iterate on individuals */
	for (iterator_individual=0; iterator_individual < number_of_individuals; iterator_individual++)
	{
		population[iterator_individual] = (int *) malloc (v_size *sizeof (int));
		/* iterate on positions of an individual */
		for (iterator_individual_position = 0; iterator_individual_position < v_size; iterator_individual_position++)
		{
			/* individual with SLA != max_SLA */
			if (V[iterator_individual_position][3] != max_SLA)
			{
				/* assign to each virtual machine a random generated physical machine */
				population[iterator_individual][iterator_individual_position] = generate_solution_position(h_size, 0);
			} 
			/* individual with SLA == max_SLA */
			if (V[iterator_individual_position][3] == max_SLA)
			{
				/* assign to each virtual machine a random generated physical machine */
				population[iterator_individual][iterator_individual_position] = generate_solution_position(h_size, 1);
			}
		}
	}
	return population;
}

/* generate_solution_position: generates a host number between 0 or 1 and parameter max_posible
 * parameter: maximun number for the randon number to return
 * parameter: SLA of the individual. for 1 the placement is mandatorly
 * returns: random number between 0 and parameter max_posible
 */
int generate_solution_position(int max_posible, int SLA)
{
	/* assign to each virtual machine a random generated physical machine from 0 to the maximum possible */
	if (SLA == 0)
	{
		/* integer from 0 to max_posible + 1*/
		return rand() % (max_posible + 1);
	} 
	/* assign to each virtual machine a random generated physical machine from 1 to the maximum possible */
	if (SLA == 1)
	{
		/* integer from 1 to max_posible */
		return rand() % max_posible + 1;
	} 
}
