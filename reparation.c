/* 
 * reparation.c: Virtual Machine Placement Problem - Reparation Stage
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include reparation stage header */
#include "reparation.h"

/* reparation: reparates the population
 * parameter: population matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * returns: reparated population matrix
 */
int** reparation(int ** population, int *** utilization, int ** H, int ** V, int number_of_individuals, int h_size, int v_size, int max_SLA, int * K, int ** network_utilization, int l_size, int ** G, int ** T)
{
	/* repairs population from not factible individuals */
	repair_population(population, utilization, H, V, number_of_individuals, h_size, v_size, max_SLA, K, network_utilization, l_size, G, T);
	return population;
}

/* repair_population: repairs population from not factible individuals
 * parameter: population matrix
 * parameter: utilization of the physical machines matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * returns: nothing, it's void()
 */
void repair_population(int ** population, int *** utilization, int ** H, int ** V, int number_of_individuals, int h_size, int v_size, int max_SLA, int * K, int ** network_utilization, int l_size, int ** G, int ** T)
{
	/* iterators */
	int iterator_individual = 0;
	int iterator_virtual = 0;
	int iterator_physical = 0;
	int iterator_link = 0;
	int factibility = 1;
	/* iterate on individuals */
	for (iterator_individual = 0; iterator_individual < number_of_individuals ; iterator_individual++)
	{
		/* every individual is feasible until it's probed other thing */		
		factibility = 1;
		/* constraint 2: Service Level Agreement (SLA) provision. Virtual machines with SLA = max_SLA have to be placed mandatorily */
		for (iterator_virtual = 0; iterator_virtual < v_size; iterator_virtual++)
		{
			if (V[iterator_virtual][3] == max_SLA && population[iterator_individual][iterator_virtual] == 0)
			{
				factibility = 0;
				break;
			}
		}
		/* constraints 3-5: Resource capacity of physical machines. Iterate on physical machines */
		for (iterator_physical = 0; iterator_physical < h_size ; iterator_physical++)
		{
			/* if any physical machine is overloaded on any resource, the individual is not factible */
			if (is_overloaded(H, utilization, iterator_individual, iterator_physical))
			{
				factibility = 0;
				break;
			}
		}

		/* if the individual is not factible */
		if (factibility == 0)
		{
			repair_individual(population, utilization, H, V, number_of_individuals, h_size, v_size, max_SLA, K, network_utilization, l_size, iterator_individual, G, T);
		}
	}
}

/* repair_individual: repairs not factible individuals
 * parameter: population matrix
 * parameter: utilization of the physical machines matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * parameter: identificator of the not factible individual to repair
 * returns: nothing, it's void()
 */
void repair_individual(int ** population, int *** utilization, int ** H, int ** V, int number_of_individuals, int h_size, int v_size, int max_SLA, int * K, int ** network_utilization, int l_size, int individual, int ** G, int ** T)
{
	int iterator_virtual = 0;
	int iterator_virtual_again=0;
	int iterator_physical = 0;	
	int iterator_link = 0;
	/* every individual is not feasible until it's probed other thing */		
	int factibility = 0;
	/* id of a candidate physical machine for migration */
	int candidate = 0;
	/* a migration flag for overloaded physical machines indicating that a virtual machine was or not migrated yet */
	int migration = 0;
	/* iterate on each virtual machine to search for overloaded physical machines */
	for (iterator_virtual = 0; iterator_virtual < v_size; iterator_virtual++)
	{
		/* if the virtual machine was placed */
		if (population[individual][iterator_virtual] != 0)
		{
			migration = 0;
			/* verify is the physical machine assigned is overloaded in any physical resource */
			if (is_overloaded(H, utilization, individual, (population[individual][iterator_virtual]-1)))
			{
				/* we search for a correct candidate for VM "migration" (it is not really a migration, only a physical machine change) */
				candidate = rand() % h_size;
				for (iterator_physical=0; iterator_physical < h_size; iterator_physical++)
				{				
					/* if the candidate can assume the resource requested */
					if (utilization[individual][candidate][0] + V[iterator_virtual][0] <= H[candidate][0] &&
	   			   	 	utilization[individual][candidate][1] + V[iterator_virtual][1] <= H[candidate][1] && 
						utilization[individual][candidate][2] + V[iterator_virtual][2] <= H[candidate][2])
					{
						/* delete requirements from physical machine migration source */
						utilization[individual][population[individual][iterator_virtual]-1][0] -= V[iterator_virtual][0];
						utilization[individual][population[individual][iterator_virtual]-1][1] -= V[iterator_virtual][1];
						utilization[individual][population[individual][iterator_virtual]-1][2] -= V[iterator_virtual][2];
						/* delete requirements from network links related to physical machine migration source */
						for (iterator_virtual_again=0; iterator_virtual_again < v_size; iterator_virtual_again++)
						{
							/* consider only traffic between virtual machines in different physical machines */
							if (population[individual][iterator_virtual] != population[individual][iterator_virtual_again] && population[individual][iterator_virtual_again] != 0)
							{
								/* for each network link */
								for (iterator_link=0; iterator_link < l_size; iterator_link++)
								{
									/* add the corresponding traffic for each used link */
									if (G[population[individual][iterator_virtual]-1][iterator_link] !=0 || 
									G[population[individual][iterator_virtual_again]-1][iterator_link] !=0)
									{	
										if(T[iterator_virtual][iterator_virtual_again]!=0)
										{
											network_utilization[individual][iterator_link] -= T[iterator_virtual][iterator_virtual_again];
										}
									}
								}
							}
						}
						/* add requirements from physical machine migration destination */
						utilization[individual][candidate][0] += V[iterator_virtual][0];
						utilization[individual][candidate][1] += V[iterator_virtual][1];
						utilization[individual][candidate][2] += V[iterator_virtual][2];
						/* refresh the population */
						population[individual][iterator_virtual] = candidate + 1;
						/* add requirements from network links related to physical machine migration destination */
						for (iterator_virtual_again=0; iterator_virtual_again < v_size; iterator_virtual_again++)
						{
							/* consider only traffic between virtual machines in different physical machines */
							if (population[individual][iterator_virtual] != population[individual][iterator_virtual_again] && population[individual][iterator_virtual_again] != 0)
							{
								/* for each network link */
								for (iterator_link=0; iterator_link < l_size; iterator_link++)
								{
									/* add the corresponding traffic for each used link */
									if (G[population[individual][iterator_virtual]-1][iterator_link] !=0 || 
									G[population[individual][iterator_virtual_again]-1][iterator_link] !=0)
									{
										if(T[iterator_virtual][iterator_virtual_again]!=0)
										{
											network_utilization[individual][iterator_link] += T[iterator_virtual][iterator_virtual_again];
										}
									}
								}
							}
						}
						/* virtual machine correctly "migrated" */
						migration = 1;
						break;
						if (candidate < h_size)
						{
							candidate++;
						}
						else
						{
							candidate = 0;
						}
					}
				}
				if (!migration)
				{
					if (V[iterator_virtual][3]!=max_SLA)
					{
						/* delete requirements from physical machine migration source */
						utilization[individual][population[individual][iterator_virtual]-1][0] -= V[iterator_virtual][0];
						utilization[individual][population[individual][iterator_virtual]-1][1] -= V[iterator_virtual][1];
						utilization[individual][population[individual][iterator_virtual]-1][2] -= V[iterator_virtual][2];
						/* delete requirements from network links related to physical machine migration source */
						for (iterator_virtual_again=0; iterator_virtual_again < v_size; iterator_virtual_again++)
						{
							/* consider only traffic between virtual machines in different physical machines */
							if (population[individual][iterator_virtual] != population[individual][iterator_virtual_again] && population[individual][iterator_virtual_again] != 0)
							{
								/* for each network link */
								for (iterator_link=0; iterator_link < l_size; iterator_link++)
								{
									/* add the corresponding traffic for each used link */
									if (G[population[individual][iterator_virtual]-1][iterator_link] !=0 || 
									G[population[individual][iterator_virtual_again]-1][iterator_link] !=0)
									{						
										if(T[iterator_virtual][iterator_virtual_again]!=0)
										{
											network_utilization[individual][iterator_link] -= T[iterator_virtual][iterator_virtual_again];
										}
									}
								}
							}
						}
						/* refresh the population */
						population[individual][iterator_virtual] = 0;
						/* virtual machine correctly "deleted" */
						migration = 1;
						break;
					}
				}
				if(!migration)
				{
					break;
				}
			}
		}
	}
}

/* is_overloaded: verificates if a physical machine is overloaded
 * parameter: physical machine resources matrix
 * parameter: utilization of the physical machines matrix
 * parameter: identificator of the individual
 * parameter: identificator of the physical machine
 * returns: 1 if yes, 0 if no
 */
int is_overloaded(int ** H, int *** utilization, int individual, int physical)
{
	/* If the use of the VM exceeds the capacity of the physical machine return 1, otherwise return 0 */
	if ((utilization[individual][physical][0] > H[physical][0]) 
	||  (utilization[individual][physical][1] > H[physical][1])
	||  (utilization[individual][physical][2] > H[physical][2]))
	{
		return 1;
	}
	return 0;
}

/* is_overassigned: verificates if a network link is overassigned
 * parameter: network link capacities array
 * parameter: utilization of the network links matrix
 * parameter: identificator of the individual
 * parameter: identificator of the link
 * returns: 1 if yes, 0 if no
 */
int is_overassigned(int *K, int **network_utilization, int individual, int link)
{
	/* If the use of the VM exceeds the capacity of the physical machine return 1, otherwise return 0 */
	if (network_utilization[individual][link] > K[link])
	{
		return 1;
	}
	return 0;
}
