/* 
 * variation.c: Virtual Machine Placement Problem - Genetic Operators Functions
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
#include "variation.h"
#include "common.h"

/* non_dominated_sorting: calculate fitness according to NSGA-II
 * parameter: solutions matrix
 * parameter: number of individuals
 * returns: array with the Pareto front
 */
int* non_dominated_sorting(float ** solutions, int number_of_individuals)
{
	/* iterators */
	int iterator_solution = 0;
	int iterator_comparision = 0;
	/* Pareto front identificator initializated to 1 */
	int actual_pareto_front = 1;
	/* number of allocated solutions */
	int solutions_allocated = 0;
	/* Pareto fronts array */
	int *pareto_fronts = (int *) malloc (number_of_individuals *sizeof (int));
	/* Pareto fronts initializated to 0 */
	for (iterator_solution=0; iterator_solution < number_of_individuals; iterator_solution++)
	{
		pareto_fronts[iterator_solution] = 0;
	}
	/* auxiliar integers */
	int dominance;
	int dont_add;
	int allocated_solutions=0;
	/* while all the solutions have been evaluated */
	while (allocated_solutions < number_of_individuals)
	{
		/* iterate on solutions */
		for (iterator_solution=0; iterator_solution < number_of_individuals; iterator_solution++)
		{
			/* flag for a solution to be added */
			dont_add = 0;
			/* compare with the actual Pareto front */
			if (pareto_fronts[iterator_solution]==0)
			{
				for (iterator_comparision=0; iterator_comparision < number_of_individuals; iterator_comparision++)
				{
					/* if the solution is not itself, it is not been evaluated or is in the actual Pareto front */
					if (iterator_solution != iterator_comparision && pareto_fronts[iterator_comparision]==0 || pareto_fronts[iterator_comparision]==actual_pareto_front)
					{
						/* verificate the dominance between both*/
						dominance = is_dominated(solutions,iterator_solution,iterator_comparision);
						/* is dominated by a solution that is in the Pareto front, so this solution is not added*/
						if (dominance == -1)
						{
							dont_add = 1;
							break;
						}
					}
				}	
				/* if the solution is not dominated by any other, let's add it to the actual Pareto front */
				if (dont_add == 0)
				{
					pareto_fronts[iterator_solution] = actual_pareto_front;
					allocated_solutions++;
				}
			}
		}
		actual_pareto_front++;
	}
	return pareto_fronts;
}

/* is_dominated: usual non-domination checking
 * parameter: solutions matrix
 * parameter: identificator of the individual a
 * parameter: identificator of the individual b
 * returns: 1 if a dominates b, -1 if b dominates a, 0 if both a and b are non-dominated 
 */
int is_dominated(float ** solutions, int a, int b)
{
	/* if a dominates b */
	/* a better in all objectives */
	if(solutions[a][0] < solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] || 
	/* a better or equal in one objective and better in the others */
	solutions[a][0] <= solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] <= solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] <= solutions[b][4] ||
	/* a better or equal in two objectives and better in the others */
	solutions[a][0] <= solutions[b][0] && solutions[a][1] <= solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] <= solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] <= solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] <= solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] <= solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] <= solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] <= solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] <= solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] <= solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] <= solutions[b][4] ||
	/* a better or equal in three objectives and better in the others */
	solutions[a][0] <= solutions[b][0] && solutions[a][1] <= solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] > solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] <= solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] <= solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] <= solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] <= solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] <= solutions[b][1] && solutions[a][2] > solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] <= solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] <= solutions[b][4] ||
	/* a better or equal in four objectives and better in the others */
	solutions[a][0] <= solutions[b][0] && solutions[a][1] <= solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] < solutions[b][4] ||
	solutions[a][0] <= solutions[b][0] && solutions[a][1] < solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] <= solutions[b][4] ||
	solutions[a][0] < solutions[b][0] && solutions[a][1] <= solutions[b][1] && solutions[a][2] >= solutions[b][2] && solutions[a][3] >= solutions[b][3] 
	&& solutions[a][4] <= solutions[b][4])
	{
		return 1;
	}

	/* if b dominates a */
	/* a better in all objectives */
	if(solutions[b][0] < solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] || 
	/* a better or equal in one objective and better in the others */
	solutions[b][0] <= solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] <= solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] <= solutions[a][4] ||
	/* a better or equal in two objectives and better in the others */
	solutions[b][0] <= solutions[a][0] && solutions[b][1] <= solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] <= solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] <= solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] <= solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] <= solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] <= solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] <= solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] <= solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] <= solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] <= solutions[a][4] ||
	/* a better or equal in three objectives and better in the others */
	solutions[b][0] <= solutions[a][0] && solutions[b][1] <= solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] > solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] <= solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] <= solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] <= solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] <= solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] <= solutions[a][1] && solutions[b][2] > solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] <= solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] <= solutions[a][4] ||
	/* a better or equal in four objectives and better in the others */
	solutions[b][0] <= solutions[a][0] && solutions[b][1] <= solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] < solutions[a][4] ||
	solutions[b][0] <= solutions[a][0] && solutions[b][1] < solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] <= solutions[a][4] ||
	solutions[b][0] < solutions[a][0] && solutions[b][1] <= solutions[a][1] && solutions[b][2] >= solutions[a][2] && solutions[b][3] >= solutions[a][3] 
	&& solutions[b][4] <= solutions[a][4])
	{
		return -1;
	}
	/* if comes here, both are non-dominated */
	return 0;
}

/* selection: selection of the parents for the crossover
 * parameter: array of the Pareto front
 * parameter: number of individuals
 * parameter: number of selection percent
 * returns: the parent for the crossover
 */
int selection(int *fronts, int number_of_individuals, float percent)
{
	/* iterator */
	int iterator_solution;
	int actual_parent;
	int posible_parent;
	/* generate randomically a parent candidate */
	actual_parent = rand() % (number_of_individuals);
	/* iterate on positions of an individual and select the parents for the crossover */
    for (iterator_solution=0; iterator_solution < (number_of_individuals * percent); iterator_solution++)
    {
		posible_parent = rand() % (number_of_individuals);
		if (fronts[actual_parent] > fronts[posible_parent])
		{
			actual_parent = posible_parent;
		}
    }
	return actual_parent;
}

/* crossover: performs the crossover operation
 * parameter: population matrix
 * parameter: the mother for the crossover
 * parameter: the father for the crossover
 * parameter: number of virtual machines
 * returns: the crossovered population
 */
int** crossover(int **population, int position_parent1, int position_parent2, int v_size)
{
  	/* iterators */
	int iterator_virtual;
	/* auxiliary parameter */
	int aux;
	/* iterate on virtual machines and performs the crossing */
	for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
	{
		/* if pair, makes the crossing in the middle, otherwise it is half + 1 */
		if (v_size % 2 == 0)
		{
			if (iterator_virtual < v_size / 2)
			{
				population[position_parent1][iterator_virtual] = population[position_parent1][iterator_virtual];
				population[position_parent2][iterator_virtual] = population[position_parent2][iterator_virtual];
			}
			else
			{
				aux = population[position_parent2][iterator_virtual];
				population[position_parent2][iterator_virtual] = population[position_parent1][iterator_virtual];
				population[position_parent1][iterator_virtual] = aux;
			}
		} 
		else
		{
			if (iterator_virtual < (v_size / 2) + 1)
			{
				population[position_parent1][iterator_virtual] = population[position_parent1][iterator_virtual];
				population[position_parent2][iterator_virtual] = population[position_parent2][iterator_virtual];
			}
			else
			{
				aux = population[position_parent2][iterator_virtual];
				population[position_parent2][iterator_virtual] = population[position_parent1][iterator_virtual];
				population[position_parent1][iterator_virtual] = aux;
			}
		}
	}
	return population;
}

/* mutation: performs the mutation operation
 * parameter: population matrix
 * parameter: number of individuals
 * parameter: number of physical_position machines
 * parameter: number of virtual machines
 * returns: the mutation population
 */
int** mutation(int **population, int **V, int number_of_individuals, int h_size, int v_size)
{
   	/* iterators */
	int iterator_virtual;
	int physical_position;
	int iterator_individual;
	/* auxiliary parameter */
	int aux;
	float probability;
	srand48(time(NULL));
	h_size + 1;
	/* iterate on individuals */
	for (iterator_individual = 0 ; iterator_individual < number_of_individuals ; iterator_individual++)
	{
		/* iterate on virtual machines */
		for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
		{
			probability = drand48() *1.0;
			/* if the probablidad is less than 1/v_size, performs the mutation */
			if (probability < (float)1/v_size)
			{
				/* get the position of the physical machine the random */
				if (V[iterator_virtual][3] == 1)
				{
					physical_position = rand() % h_size + 1;
				}
				else 
				{
					physical_position = rand() % h_size;
				}
				/* performs the mutation operation */
				if (physical_position != population[iterator_individual][iterator_virtual])
					population[iterator_individual][iterator_virtual] = physical_position;
				else
				{
					aux = population[iterator_individual][iterator_virtual];
					while (physical_position == aux)
					{
						/* individual with SLA = 1 */
						if (V[iterator_virtual][3] == 1)
						{
							physical_position = rand() % h_size + 1;
						}
						/* individual with SLA = 0 */
						else 
						{
							physical_position = rand() % h_size;
						}
						if (physical_position != population[iterator_individual][iterator_virtual]) 
							population[iterator_individual][iterator_virtual] = physical_position;
					}
				}
			}
		}
	}
	return population;
}

/* population_evolution: update the pareto front in the population
 * parameter: population matrix
 * parameter: evolutionated population matrix
 * parameter: the cost of each objetives the population matrix
 * parameter: the cost of each objetives evolutionated population matrix
 * parameter: front pareto array
 * parameter: number of individuals
 * parameter: number of virtual machines
 * returns: population matrix
 */
int** population_evolution(int **P, int **Q, float **objectives_functions_P, float **objectives_functions_Q, int *fronts_P, int number_of_individuals, int v_size)
{
	/* P union Q population matrix */
	int **PQ = (int **) malloc (2 * number_of_individuals *sizeof (int *));
	/* P union Q objectives functions values */
	float **objectives_functions_PQ = (float **) malloc (2 * number_of_individuals *sizeof (float *));
	/* iterators */
	int iterator_individual_P = 0;
	int iterator_individual_Q = 0;
	int iterator_individual_position = 0;
	/* iterate on positions of an individual and copy the P individual and objective function */
	for (iterator_individual_P=0; iterator_individual_P < number_of_individuals; iterator_individual_P++)
	{
		PQ[iterator_individual_P] = (int *) malloc (v_size *sizeof (int));
		objectives_functions_PQ[iterator_individual_P] = (float *) malloc (3 *sizeof (float));
		for (iterator_individual_position = 0; iterator_individual_position < v_size; iterator_individual_position++)
		{
			PQ[iterator_individual_P][iterator_individual_position] = P[iterator_individual_P][iterator_individual_position];
		}
		objectives_functions_PQ[iterator_individual_P][0] = objectives_functions_P[iterator_individual_P][0];
		objectives_functions_PQ[iterator_individual_P][1] = objectives_functions_P[iterator_individual_P][1];
		objectives_functions_PQ[iterator_individual_P][2] = objectives_functions_P[iterator_individual_P][2];
	}
	/* iterate on positions of an individual and copy the Q individual and objective function */
	for (iterator_individual_P=number_of_individuals; iterator_individual_P < 2 * number_of_individuals; iterator_individual_P++)
	{
		PQ[iterator_individual_P] = (int *) malloc (v_size *sizeof (int));
		objectives_functions_PQ[iterator_individual_P] = (float *) malloc (3 *sizeof (float));
		for (iterator_individual_position = 0; iterator_individual_position < v_size; iterator_individual_position++)
		{
			PQ[iterator_individual_P][iterator_individual_position] = Q[iterator_individual_Q][iterator_individual_position];
		}
		objectives_functions_PQ[iterator_individual_P][0] = objectives_functions_Q[iterator_individual_Q][0];
		objectives_functions_PQ[iterator_individual_P][1] = objectives_functions_Q[iterator_individual_Q][1];
		objectives_functions_PQ[iterator_individual_P][2] = objectives_functions_Q[iterator_individual_Q][2];
		iterator_individual_Q++;
	}

	/* calculate fitness according to NSGA-II */
	int *fronts_PQ = non_dominated_sorting(objectives_functions_PQ, number_of_individuals*2);
	int *population_aux = (int *) malloc (v_size *sizeof (int));
	/* generate Pt+1 according to NSGA-II */
	int iterator;
	int iterator_virtual;
	int iterator_P = 0;
	int actual_pareto = 0;
	int i,j;
	int iterator_solution;
	/* generate Pt+1 according to NSGA-II */
	while (iterator_P < number_of_individuals)
	{
		actual_pareto++;
		for (iterator = 0; iterator < number_of_individuals*2 ; iterator++)
		{
			if (fronts_PQ[iterator] == actual_pareto && iterator_P < number_of_individuals)
			{
				if (objectives_functions_PQ[iterator][0] != 0 || objectives_functions_PQ[iterator][1] != 0 ||
			objectives_functions_PQ[iterator][2] !=0)
				{
					objectives_functions_P[iterator_P][0] = objectives_functions_PQ[iterator][0];
					objectives_functions_P[iterator_P][1] = objectives_functions_PQ[iterator][1];
					objectives_functions_P[iterator_P][2] = objectives_functions_PQ[iterator][2];
					fronts_P[iterator_P] = fronts_PQ[iterator];
								
					for (iterator_virtual = 0; iterator_virtual<v_size ; iterator_virtual++)
					{
						P[iterator_P][iterator_virtual] = PQ[iterator][iterator_virtual];
					}
					iterator_P++;
				}
			}
		}
	}
	return P;
}
