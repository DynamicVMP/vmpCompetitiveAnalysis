/*
 * variation.c: Virtual Machine Placement Problem - Genetic Operators Functions
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include libraries */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* include headers*/
#include "variation.h"

/* selection: selection of the parents for the crossover using binary tournament
 * parameter: array of the evaluation of each individual solution
 * parameter: number or individuals
 * returns: the parent for the crossover
 */
int selection(double *objective_function, int number_of_individuals)
{
    /* iterator */
    int iterator_solution;
    int actual_parent;
    int posible_parent;
    /* generate randomically a parent candidate */
    actual_parent = rand() % (number_of_individuals);
    /* iterate on positions of an individual and select the parents for the crossover */
    for (iterator_solution=0; iterator_solution < TOURNAMENT_SIZE - 1; iterator_solution++)
    {
        posible_parent = rand() % (number_of_individuals);

        if (objective_function[actual_parent] < objective_function[posible_parent])
        {
            actual_parent = posible_parent;
        }
    }
    return actual_parent;
}


/* selection_and_crossover: select individuals from P to performs the crossover operation and insert them in the population Q
 * parameter: population matrix Q
 * parameter: population matrix P
 * parameter: the weighted sums
 * parameter: the number of individuals
 * parameter: number of virtual machines
 * returns: the population after apply the crossover
 */
int** selection_and_crossover(int **populationQ, int **populationP, double *weighted_sums_P, int number_of_individuals,
                             int v_size){

    int iterator,father,mother;

    /*  selection of solutions from P */
    for(iterator=0;iterator<number_of_individuals/2;iterator++){
        father = selection(weighted_sums_P, number_of_individuals);
        mother = selection(weighted_sums_P, number_of_individuals);
        //printf("\nfather %d\n",father);
        // printf("\nmother %d\n",mother);
        /* Q = crossover solutions */
        populationQ = crossover_individuals(populationQ,populationP,father, mother, v_size);
        //print_int_matrix(Q,number_of_individuals,v_size);
    }

    return populationQ;

}

/* crossover: performs the crossover operation of individuals
 * parameter: population matrix
 * parameter: the mother for the crossover
 * parameter: the father for the crossover
 * parameter: number of virtual machines
 * returns: the population after apply the crossover
 */
int** crossover_individuals(int **populationQ, int **populationP, int position_parent1, int position_parent2, int v_size)
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
                populationQ[position_parent1][iterator_virtual] = populationP[position_parent1][iterator_virtual];
                populationQ[position_parent2][iterator_virtual] = populationP[position_parent2][iterator_virtual];
            }
            else
            {
                aux = populationP[position_parent2][iterator_virtual];
                populationQ[position_parent2][iterator_virtual] = populationP[position_parent1][iterator_virtual];
                populationQ[position_parent1][iterator_virtual] = aux;
            }
        }
        else
        {
            if (iterator_virtual < (v_size / 2) + 1)
            {
                populationQ[position_parent1][iterator_virtual] = populationP[position_parent1][iterator_virtual];
                populationQ[position_parent2][iterator_virtual] = populationP[position_parent2][iterator_virtual];
            }
            else
            {
                aux = populationP[position_parent2][iterator_virtual];
                populationQ[position_parent2][iterator_virtual] = populationP[position_parent1][iterator_virtual];
                populationQ[position_parent1][iterator_virtual] = aux;
            }
        }
    }

    return populationQ;
}

/* mutation: performs the mutation operation
 * parameter: population matrix
 * parameter: number of individuals
 * parameter: number of physical_position machines
 * parameter: number of virtual machines
 * returns: the mutation population
 */
int** mutation(int **population, float **V, int number_of_individuals, int h_size, int v_size, int max_SLA)
{
    /* iterators */
    int iterator_virtual;
    int physical_position;
    int iterator_individual;
    /* auxiliary parameter */
    int aux;
    float probability;
    srand48(time(NULL));

    /* iterate on individuals */
    for (iterator_individual = 0 ; iterator_individual < number_of_individuals ; iterator_individual++)
    {
        /* iterate on virtual machines */
        for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
        {
            probability = drand48() *1.0;

            /* if the vm is not marked as off (when CPU > 0) of the vm was derived, we do nothing */
            if(V[iterator_virtual][0]>0 && V[iterator_virtual]==NOT_DERIVED) {
                /* if the probablidad is less than 1/v_size, performs the mutation */
                if (probability < (float) 1 / v_size) {
                    /* get the position of the physical machine the random*/
                    /*if (V[iterator_virtual][3] == max_SLA)
                    {
                        physical_position = rand() % h_size + 1;
                    }
                    else
                    {
                        physical_position = rand() % h_size;
                    }*/

                    physical_position = rand() % h_size + 1;

                    /* performs the mutation operation */
                    if (physical_position != population[iterator_individual][iterator_virtual])
                        population[iterator_individual][iterator_virtual] = physical_position;
                    else {
                        aux = population[iterator_individual][iterator_virtual];
                        while (physical_position == aux && h_size > 1) {
                            /*
                             * individual with SLA = max_SLA*/
                            /*if (V[iterator_virtual][3] == max_SLA)
                            {
                                physical_position = rand() % h_size + 1;
                            }
                            else
                            {
                                physical_position = rand() % h_size;
                            }*/
                            physical_position = rand() % h_size + 1;
                            if (physical_position != population[iterator_individual][iterator_virtual])
                                population[iterator_individual][iterator_virtual] = physical_position;
                        }
                    }
                }
            }
        }
    }
    return population;
}

/* population_evolution: update the population P
 * parameter: population matrix P
 * parameter: evolved population matrix Q
 * parameter: the cost of the objective function of the population matrix P
 * parameter: the cost of the objective function of population matrix Q
 * parameter: number of individual
 * parameter: number of virtual machines
 * returns: population matrix P
 */
int** population_evolution(int **P, int **Q, double *weighted_sums_P, double *weighted_sums_Q, int number_of_individuals, int v_size)
{

    /* P union Q population matrix */
    int **PQ = (int **) malloc (2 * number_of_individuals *sizeof (int *));
    /* P union Q objectives functions values */
    double *objective_function_PQ = (double *) malloc (2 * number_of_individuals *sizeof (double));

    /* iterators */
    int iterator_individual_P = 0;
    int iterator_individual_Q = 0;
    int iterator_individual_position = 0;

    /* iterate on positions of an individual and copy the P individual and objective function */
    for (iterator_individual_P=0; iterator_individual_P < number_of_individuals; iterator_individual_P++)
    {
        PQ[iterator_individual_P] = (int *) malloc (v_size *sizeof (int));
        for (iterator_individual_position = 0; iterator_individual_position < v_size; iterator_individual_position++)
        {
            PQ[iterator_individual_P][iterator_individual_position] = P[iterator_individual_P][iterator_individual_position];
        }
        objective_function_PQ[iterator_individual_P] = weighted_sums_P[iterator_individual_P];
    }

    /* iterate on positions of an individual and copy the Q individual and objective function */
    for (iterator_individual_P=number_of_individuals; iterator_individual_P < 2 * number_of_individuals; iterator_individual_P++)
    {
        PQ[iterator_individual_P] = (int *) malloc (v_size *sizeof (int));
        for (iterator_individual_position = 0; iterator_individual_position < v_size; iterator_individual_position++)
        {
            PQ[iterator_individual_P][iterator_individual_position] = Q[iterator_individual_Q][iterator_individual_position];
        }
        objective_function_PQ[iterator_individual_P] = weighted_sums_Q[iterator_individual_Q];
        iterator_individual_Q++;
    }

    //print_float_array(objective_function_PQ,number_of_individuals*2);
    // print_int_matrix(PQ,number_of_individuals*2,v_size);
    //printf("\n\n");
    /*sort the union of the population P and Q by the value of the objective function*/
    if(v_size>0) {
        quicksort_sort(objective_function_PQ, PQ, 0, number_of_individuals * 2 - 1);
    }
    // print_float_array(objective_function_PQ,number_of_individuals*2);
    //print_int_matrix(PQ,number_of_individuals*2,v_size);
    // printf("\n\n");

    /*The n_individuals with the best values ​​of objective function are selected for the next generation of the population P*/
    for (iterator_individual_P=0; iterator_individual_P < number_of_individuals; iterator_individual_P++)
    {
        for (iterator_individual_position = 0; iterator_individual_position < v_size; iterator_individual_position++)
        {
            P[iterator_individual_P][iterator_individual_position]=PQ[iterator_individual_P][iterator_individual_position];
        }
    }

    //printf("\n New Population P\n");
    //print_int_matrix(P,number_of_individuals,v_size);

    /*free the memory allocated for the auxiliary structures*/
    free_int_matrix(PQ,number_of_individuals);
    free(objective_function_PQ);

    return P;
}




/**
 * quicksort_decreasing_sort: Sort the population with quicksort in-place based on the weight sums of the solutions
 * parameter weighted_sums: the values to use to sort the population
 * parameter population: population to sort
 * parameter first_index: index of the first element in population
 * parameter last_index: index of the last element in population
 *
 * returns: nothing, it's a void function.
 */
void quicksort_sort(double *weighted_sums,int ** population, int first_index, int last_index){

    int pivot_index = 0;

    if(first_index < last_index) {
        pivot_index = quicksort_partition(weighted_sums,population,first_index, last_index);
        quicksort_sort(weighted_sums,population, first_index, (pivot_index - 1));
        quicksort_sort(weighted_sums,population, (pivot_index + 1), last_index);
    }

}


/**
 * quicksort_partition: partition of the quicksort algorithm
 * parameter weighted_sums: the values to use to sort the population
 * parameter population: the population to sort
 * parameter first_index: index of the first element in weighted sums
 * parameter last_index: index of the last element in weighted sums
 *
 * returns: the position of the pivot to use.
 */
int quicksort_partition(double *weighted_sums,int ** population, int first_index, int last_index){

    int up_index, down_index;
    bool first_iteration = true;
    double pivot, temp;
    int* temp_indv;
    int* pivot_indv;

    pivot =  weighted_sums[first_index];
    pivot_indv = population[first_index];
    up_index = first_index;
    down_index = last_index;

    do {
        // If is not the first iteration, to avoid the use of goto statements
        if(!first_iteration) {
            temp =  weighted_sums[up_index];
            weighted_sums[up_index]= weighted_sums[down_index];
            weighted_sums[down_index] = temp;

            temp_indv = population[up_index];
            population[up_index] = population[down_index];
            population[down_index] = temp_indv;

        }
        first_iteration = false;

        // While the up element is better or equal than the selected pivot
        while (weighted_sums[up_index]<= pivot && up_index < last_index) {
            up_index++;
        }

        // While the down element is worst than the selected pivot
        while (weighted_sums[down_index] > pivot   && down_index > first_index ) {
            down_index--;
        }

    } while (down_index > up_index);

    if(weighted_sums[first_index] > weighted_sums[down_index]) {
        weighted_sums[first_index]= weighted_sums[down_index];
        weighted_sums[down_index] = pivot;

        population[first_index] = population[down_index];
        population[down_index] = pivot_indv;
    }


    return down_index;

}
