/*
 * initialization.c: Virtual Machine Placement Problem - Initialization Stage
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include initialization stage header */
#include "initialization.h"

/* initialization: initializes a population randomly
 * parameter: the population matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * parameter: virtual machines requirements matrix
 * parameter: the max level of SLA
 * returns: population matrix
 */
int** initialization(int** population, int number_of_individuals, int h_size, int v_size,int previous_v_size, float **V, int max_SLA)
{
    /* iterators */
    int iterator_individual;
    int iterator_individual_position;
    /* iterate on individuals */
    for (iterator_individual=0; iterator_individual < number_of_individuals; iterator_individual++)
    {
        /* iterate on positions of an individual */
        for (iterator_individual_position = 0; iterator_individual_position < v_size; iterator_individual_position++)
        {
            /*if a vm is considered*/
            if(V[iterator_individual_position][0]>0 && V[iterator_individual_position][10]==NOT_DERIVED){

                if(iterator_individual_position>=previous_v_size){
                    population[iterator_individual][iterator_individual_position] = generate_solution_position(h_size, 0);
                }else{
                    population[iterator_individual][iterator_individual_position] = generate_solution_position(h_size, 1);
                }
                /*individual with SLA != max_SLA */
                //(if (V[iterator_individual_position][3] != max_SLA)
                //{
                    /*assign to each virtual machine a random generated physical machine*/
                    //population[iterator_individual][iterator_individual_position] = generate_solution_position(h_size, 0);
                //}
                /*individual with SLA == max_SLA*/
                //if (V[iterator_individual_position][3] == max_SLA)
                //{
                    /*assign to each virtual machine a random generated physical machine*/
                //    population[iterator_individual][iterator_individual_position] = generate_solution_position(h_size, 1);
                //}

            }else{
                population[iterator_individual][iterator_individual_position] = 0;
            }

        }
    }
    return population;
}

/* generate_solution_position: generates a host number between 0 or 1 and parameter max_posible
 * parameter: maximum number for the random number to return
 * parameter: SLA of the individual. for max_SLA the placement is mandatory
 * returns: random number between 0 and parameter max_posible
 */
int generate_solution_position(int max_posible, int option)
{
    /*assign to each virtual machine a random generated physical machine from 0 to the maximum possible*/
    if (option == 0)
    {
        /*integer from 0 to max_posible + 1*/
        return rand() % (max_posible + 1);
    }
    /*assign to each virtual machine a random generated physical machine from 1 to the maximum possible*/
    if (option == 1)
    {
        /*integer from 1 to max_posible*/
        return rand() % max_posible + 1;
    }
}

/* create_structures: allocate memory for structures
 * parameter: population P matrix
 * parameter: population Q matrix
 * parameter: utilization of population P
 * parameter: utilization of population Q
 * parameter: objective function values P
 * parameter: objective function values Q
 * parameter: number of individuals
 * parameter: number of virtual machines
 * parameter: number of physical machines
 * returns: nothing, it's void
 */
void create_structures(int*** population_P, int*** population_Q, int**** utilization_P, int**** utilization_Q, double** weighted_sums_P,
                       double** weighted_sums_Q, double***objectives_functions_values, float*** wasted_resources_obj, int number_of_individuals, int v_size, int h_size){

    int iterator_individual, iterator_physical;

    /*allocate memory for the matrix that  holds the value of each objective function for each individual */
    *objectives_functions_values = (double**)malloc(number_of_individuals * sizeof(double*));

    *wasted_resources_obj  = (float**)malloc(number_of_individuals*sizeof(float*));

    /*allocate memory for the populations*/
    *population_P = (int**)malloc(number_of_individuals*sizeof(int*));
    *population_Q = (int**)malloc(number_of_individuals*sizeof(int*));

    for(iterator_individual=0;iterator_individual<number_of_individuals;iterator_individual++){
        (*population_P)[iterator_individual] = (int*)malloc(v_size*sizeof(int));
        (*population_Q)[iterator_individual] = (int*)malloc(v_size*sizeof(int));

        /*4 objective functions*/
        (*objectives_functions_values)[iterator_individual] = (double*)malloc(OBJECTIVE_FUNCTIONS * sizeof(double));

        (*wasted_resources_obj)[iterator_individual] = (float*)malloc(RESOURCES*sizeof(float));
    }

    /*allocate memory for the structure that holds the utilization*/

    *utilization_P = (int***)malloc(number_of_individuals*sizeof(int**));
    *utilization_Q = (int***)malloc(number_of_individuals*sizeof(int**));

    for(iterator_individual=0;iterator_individual<number_of_individuals;iterator_individual++){
        (*utilization_P)[iterator_individual] = (int**)malloc(h_size*sizeof(int*));
        (*utilization_Q)[iterator_individual] = (int**)malloc(h_size*sizeof(int*));
        for(iterator_physical=0;iterator_physical<h_size;iterator_physical++){
            (*utilization_P)[iterator_individual][iterator_physical] = (int*)malloc(RESOURCES*sizeof(int));
            (*utilization_Q)[iterator_individual][iterator_physical] = (int*)malloc(RESOURCES*sizeof(int));
        }
    }

    /*allocate memory for the structure that holds the weighted sums each individual*/
    *weighted_sums_P = (double*) malloc(number_of_individuals * sizeof(double));
    *weighted_sums_Q = (double*) malloc(number_of_individuals * sizeof(double));

}
