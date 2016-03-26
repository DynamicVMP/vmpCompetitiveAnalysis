/*
 * reparation.h: Virtual Machine Placement Problem - Reparation Stage Header
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

/* function headers definitions */
int** reparation(int ** population, int *** utilization, int ** H, float ** V, int number_of_individuals, int h_size, int v_size, int max_SLA);
void repair_population(int ** population, int *** utilization, int ** H, float ** V, int number_of_individuals, int h_size, int v_size, int max_SLA);
void repair_individual(int ** population, int *** utilization, int ** H, float ** V, int number_of_individuals, int h_size, int v_size, int max_SLA, int individual);
int is_overloaded(int **H, int ***utilization, int individual, int physical);
int check_feasibility(int** population,int *** utilization,int iterator_individual,int ** H, float ** V,int h_size,int v_size,int max_SLA);
int check_pm_capacity(int ***utilization, int individual, int candidate, float **V, int virtual_machine, int **H);
int generate_unique_candidate(int service, int vm_index, int **population, int individual, int ***utilization, float **V, int v_size, int **H, int h_size);