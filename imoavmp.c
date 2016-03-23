/*
 * imavmp.c: Interactive Memetic Algorithm for Virtual Machine Placement (IMAVMP)
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
/* include own headers */
#include "common.h"
#include "initialization.h"
#include "reparation.h"
#include "local_search.h"
#include "variation.h"
#include "scenario.h"

/* definitions (this could be parameters) */
#define NUMBER_OF_INDIVIDUALS 10000
#define NUMBER_OF_GENERATIONS 1

#define MAX_SLA 4


void save_objective_functions(float** objectives_functions_values_aux,int number_of_individuals);

/* main: Interactive Memetic Algorithm for Virtual Machine Placement(IMAVMP)
 * parameter: path to the datacenter infrastructure file
 * returns: exit state
 */
int main (int argc, char *argv[]) {
    /* parameters verification */
    if (argc!=3)
    {
        /* wrong parameters */
        printf("[ERROR] usage: %s datacenter_file time_t\n", argv[0]);
        /* finish him */
        return 1;
    }
        /* good parameters */
    else
    {

        /* Interactive Memetic Algorithm previous stuff */
        /* number of generation, for iterative reference of generations */
        int generation = 0;
        /* get the number of physical machines, virtual machines and the scenario from the infrastructure file (argv[1]) */
        int h_size = get_h_size(argv[1]);
        int t_max = atoi(argv[2]);
        int s_size = get_s_size(argv[1]);

        // printf("\nH=%d, V=%d, L=%d\n",h_size,v_size,l_size);
        /* load physical machines resources, virtual machines requirements and scenario from the datacenter infrastructure file */
        int **H = load_H(h_size, argv[1]);
        float **S = load_S(s_size, argv[1]);


        int v_size;
        float **V;

        /*printf("\nH\n\n");
        print_int_matrix(H,h_size,4);*/

        /*printf("\nS\n\n");
        print_float_matrix(S,s_size,14);*/

        /* seed for rand() */
        srand((unsigned int) time(NULL));
        /* randon value of 0-1 */
        srand48(time(NULL));

        /* Interactive Memetic Algorithm starts here */
        /* 01: Check if the problem has a solution */
        if (check_instance() != 0)
        {
            /* no solution was found */
            printf("\nThe problem has no solution, call Amazon EC2\n");
            return 1;
        }

        /*timer*/
        clock_t start;
        clock_t diff;
        int msec=0,total_time=0;

        /*the index of the best solution in the population*/
        int index_best_solution;
        /*the best solution for the problem instance*/
        int* best_solution;

        float** objectives_functions_values_aux;

        /* Additional task: structures for Q and P  */
        int **Q;
        int ***utilization_Q;
        float *weighted_sums_Q;
        int **P;
        int ***utilization_P;
        float *weighted_sums_P;
        int t=1;
        int OF_calc_count=0;

        /* While t is less or equal to t_max of the scenario, execute the memetic algorithm for the time t */
        while(t<=t_max) {

            v_size = get_v_size_per_t(S,t,s_size);
            V = load_v_per_t(S,s_size,v_size,t);

            //printf("\nV\n\n");
            //print_float_matrix(V,v_size,5);

            /*reserve memory for the matrix and arrays used by the memetic algorithm*/
            create_structures(&P, &Q, &utilization_P, &utilization_Q, &weighted_sums_P, &weighted_sums_Q,
                              &objectives_functions_values_aux, NUMBER_OF_INDIVIDUALS, v_size, h_size);

            // Set Timer
            start = clock();

            /* the problem instance have at least one solution, so we can continue */
            /* Initialize population P_0 */
            P = initialization(P, NUMBER_OF_INDIVIDUALS, h_size, v_size, V, MAX_SLA);
            //print_int_matrix(P,NUMBER_OF_INDIVIDUALS,v_size);

            /* Additional task: load the utilization of physical machines and network links of all individuals/solutions */
            utilization_P = load_utilization(utilization_P, P, H, V, NUMBER_OF_INDIVIDUALS, h_size, v_size);

            /* P0’ = repair infeasible solutions of P_0 */
            P = reparation(P, utilization_P, H, V, NUMBER_OF_INDIVIDUALS, h_size, v_size, MAX_SLA);

            /*  apply local search to solutions of P_0’ */
            P = local_search(P, utilization_P, H, V, NUMBER_OF_INDIVIDUALS, h_size, v_size);
            //print_int_matrix(P,NUMBER_OF_INDIVIDUALS,v_size);

            /* Additional task: calculate the cost of each objective function for each solution */
            weighted_sums_P = load_weighted_sums(objectives_functions_values_aux, weighted_sums_P, P, utilization_P, H, V,
                                                 NUMBER_OF_INDIVIDUALS, h_size, v_size,&OF_calc_count);

            generation=0;
            /* While (stopping criterion is not met), do */
            while (generation < NUMBER_OF_GENERATIONS) {
                /* this is a new generation! */
                generation++;
                /* Additional task: Q_u is a random generated population, lets initialize it */
                Q = initialization(Q, NUMBER_OF_INDIVIDUALS, h_size, v_size, V, MAX_SLA);
                //print_int_matrix(Q,NUMBER_OF_INDIVIDUALS,v_size);
                /* Q_u=selection and crossover of solutions of P_u */
                Q = selection_and_crossover(Q, P, weighted_sums_P, NUMBER_OF_INDIVIDUALS, v_size);

                /* Q_u'=mutation of solutions of Q_u */
                Q = mutation(Q, V, NUMBER_OF_INDIVIDUALS, h_size, v_size, MAX_SLA);

                /* Additional task: load the utilization of physical machines and network links of all individuals/solutions */
                utilization_Q = load_utilization(utilization_Q, Q, H, V, NUMBER_OF_INDIVIDUALS, h_size, v_size);

                /* Q_u'' = repair infeasible solutions of Q_u' */
                Q = reparation(Q, utilization_Q, H, V, NUMBER_OF_INDIVIDUALS, h_size, v_size, MAX_SLA);

                /* Q_u'''= apply local search to solutions of Q_u'' */
                Q = local_search(Q, utilization_Q, H, V, NUMBER_OF_INDIVIDUALS, h_size, v_size);

                //printf("\nQ CREATED SUCCESSFULLY G:%d\n\n",generation);
                //print_int_matrix(Q,NUMBER_OF_INDIVIDUALS,v_size);

                /* Additional task: calculate the cost of each objective function for each solution */
                weighted_sums_Q = load_weighted_sums(objectives_functions_values_aux, weighted_sums_Q, Q, utilization_Q,
                                                     H,
                                                     V, NUMBER_OF_INDIVIDUALS, h_size, v_size,&OF_calc_count);
                //print_float_array(objective_function_Q,NUMBER_OF_INDIVIDUALS);

                /**/
                P = population_evolution(P, Q, weighted_sums_P, weighted_sums_Q, NUMBER_OF_INDIVIDUALS, v_size);

                /*Reload the utilization of P and the evaluation of each individual of P*/
                utilization_P = load_utilization(utilization_P, P, H, V, NUMBER_OF_INDIVIDUALS, h_size, v_size);
                weighted_sums_P = load_weighted_sums(objectives_functions_values_aux, weighted_sums_P, P, utilization_P,
                                                     H,
                                                     V, NUMBER_OF_INDIVIDUALS, h_size, v_size,&OF_calc_count);

            }

            index_best_solution = get_best_solution_index(weighted_sums_P, NUMBER_OF_INDIVIDUALS);
            best_solution = P[index_best_solution];


            /*calculates the time taken*/
            diff = clock() - start;
            msec = diff * 1000 / CLOCKS_PER_SEC;
            total_time+=msec;

            report_solution(best_solution, utilization_P[index_best_solution], weighted_sums_P[index_best_solution],
                            h_size, v_size, t);

            // RESULTS
            printf("\nFINAL - PLACEMENT for time T=%d\n", t);
            print_int_array(best_solution, v_size);
            printf("\nFINAL - OBJECTIVE FUNCTION VALUE for time T=%d\n", t);
            printf("%.2f\n\n", weighted_sums_P[index_best_solution]);
            printf("\nRESULTS for T=%d\n",t);
            printf("Time taken %d seconds %d milliseconds\n", msec / 1000, msec % 1000);

            /*cleaning*/
            free_int_matrix(P, NUMBER_OF_INDIVIDUALS);
            free_float_matrix(V, v_size);
            free_int_matrix(Q, NUMBER_OF_INDIVIDUALS);
            free(weighted_sums_P);
            free(weighted_sums_Q);
            free_float_matrix(objectives_functions_values_aux, NUMBER_OF_INDIVIDUALS);
            free_utilization_matrix(utilization_P, NUMBER_OF_INDIVIDUALS, h_size);
            free_utilization_matrix(utilization_Q, NUMBER_OF_INDIVIDUALS, h_size);

            t++;
        }
        free_int_matrix(H, h_size);
        free_float_matrix(S, s_size);

        printf("\nRESULTS\n");
        printf("Total Time taken %d seconds %d milliseconds\n", total_time / 1000, total_time % 1000);
        printf("Number of times the objective function was assessed: %d\n", OF_calc_count);


        /* finish him */
        return 0;
    }
}


void save_objective_functions(float ** objectives_functions_values_aux,int number_of_individuals){

    FILE *objectives_functions_file;

    int iterator,iterator2;
    objectives_functions_file = fopen("results/objective_functions","w");

    //fprintf(solutions_t,"Final placement obtained for t=%d\n",t);

    for(iterator=0;iterator<number_of_individuals;iterator++){

        for(iterator2=0;iterator2<4;iterator2++) {
            fprintf(objectives_functions_file,"%.4f\t", objectives_functions_values_aux[iterator][iterator2]);
        }
        fprintf(objectives_functions_file,"\n");
    }

}

int check_instance(){
    return 0;
}



