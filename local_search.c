/*
 * local_search.c: Virtual Machine Placement Problem - Local Search Stage
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include local search stage header */
#include "local_search.h"
#include "common.h"

/* local_search: local optimization of the population
 * parameter: population matrix
 * parameter: utilization matrix of the physical machines of all the individuals
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * returns: optimized population matrix
 */
int** local_search(int **population, int ***utilization, int **H, float **V, int number_of_individuals, int h_size, int v_size)
{
    /* iterators */
    int iterator_individual = 0;
    int physical_position = 0;
    int physical_position2 = 0;
    int iterator_virtual = 0;
    int iterator_virtual2 = 0;
    int option = 0;
    /* randon value */
    float val_rand;
    /* value to executed */
    int option_to_execute;
    val_rand = drand48() *1.0;

    if (val_rand > 0 && val_rand <= 0.5)
        option_to_execute = 0;
    else
        option_to_execute = 3;

    while(option < 2)
    {
        /* if val_rand is equal to or less than 5 is executed 1 and 2, otherwise is executed 2 and 1 */
        if (val_rand > 0 && val_rand <= 0.5)
        {
            option_to_execute++;
            option++;
        } else if (val_rand > 0.5 && val_rand < 1.0)
        {
            option_to_execute--;
            option++;
        }
        /* comes of shut down physical machines to the migrate in other virtual machines */
        if (option_to_execute == 1)
        {
            /* iterate on individuals */
            for (iterator_individual = 0 ; iterator_individual < number_of_individuals ; iterator_individual++)
            {
                /* iterate on virtual machines */
                for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
                {
                    /* get the position of the physical machine */
                    physical_position = population[iterator_individual][iterator_virtual];

                    if (physical_position > 0)
                    {
                        /* iterate on virtual machines */
                        for (iterator_virtual2 = iterator_virtual+1 ; iterator_virtual2 < v_size ; iterator_virtual2++)
                        {
                            /* get the position of the physical machine */
                            physical_position2 = population[iterator_individual][iterator_virtual2];

                            if (physical_position2 > 0)
                            {
                                if (physical_position != physical_position2 && V[iterator_virtual][7]!=V[iterator_virtual2][7])
                                {
                                    /* If the use of the VM not exceeds the capacity of the physical machine performs the migration */
                                    if (utilization[iterator_individual][physical_position-1][0] + V[iterator_virtual2][0] < H[physical_position-1][0]
                                        && utilization[iterator_individual][physical_position-1][1] + V[iterator_virtual2][1] < H[physical_position-1][1]
                                        && 	utilization[iterator_individual][physical_position-1][2] + V[iterator_virtual2][2] < H[physical_position-1][2])
                                    {
                                        /* refresh the utilization */
                                        utilization[iterator_individual][physical_position2-1][0] -= V[iterator_virtual2][0];
                                        utilization[iterator_individual][physical_position2-1][1] -= V[iterator_virtual2][1];
                                        utilization[iterator_individual][physical_position2-1][2] -= V[iterator_virtual2][2];

                                        utilization[iterator_individual][physical_position-1][0] += V[iterator_virtual2][0];
                                        utilization[iterator_individual][physical_position-1][1] += V[iterator_virtual2][1];
                                        utilization[iterator_individual][physical_position-1][2] += V[iterator_virtual2][2];
                                        /* refresh the population */
                                        population[iterator_individual][iterator_virtual2] = population[iterator_individual][iterator_virtual];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        /*  try to place the virtual machines turned off in the physical machines turned on */
        if (option_to_execute == 2)
        {
            /* iterate on individuals */
            for (iterator_individual = 0 ; iterator_individual < number_of_individuals ; iterator_individual++)
            {
                /* iterate on virtual machines */
                for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
                {
                    /* get the position of the physical machine */
                    physical_position = population[iterator_individual][iterator_virtual];

                    /*checks if a vm is no derived, is considered for the placement and wasn't placed*/
                    if (physical_position == 0 && V[iterator_virtual][0]>0 && V[iterator_virtual][10]==NOT_DERIVED)
                    {
                        /* iterate on virtual machines */
                        for (iterator_virtual2 = 0 ; iterator_virtual2 < v_size ; iterator_virtual2++)
                        {
                            /* get the position of the physical machine */
                            physical_position2 = population[iterator_individual][iterator_virtual2];

                            if (physical_position2 > 0 && V[iterator_virtual][7]!=V[iterator_virtual2][7])
                            {
                                /* If the use of the VM not exceeds the capacity of the physical machine performs the migration */
                                if (utilization[iterator_individual][physical_position2-1][0] + V[iterator_virtual][0] < H[physical_position2-1][0]
                                    && utilization[iterator_individual][physical_position2-1][1] + V[iterator_virtual][1] < H[physical_position2-1][1]
                                    && 	utilization[iterator_individual][physical_position2-1][2] + V[iterator_virtual][2] < H[physical_position2-1][2])
                                {
                                    /* refresh the utilization */
                                    utilization[iterator_individual][physical_position2-1][0] += V[iterator_virtual][0];
                                    utilization[iterator_individual][physical_position2-1][1] += V[iterator_virtual][1];
                                    utilization[iterator_individual][physical_position2-1][2] += V[iterator_virtual][2];
                                    /* refresh the population */
                                    population[iterator_individual][iterator_virtual] = population[iterator_individual][iterator_virtual2];

                                    iterator_virtual2 = v_size;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return population;
}
