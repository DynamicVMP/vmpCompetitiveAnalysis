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
int** reparation(int ** population, int *** utilization, int ** H, int ** V, int number_of_individuals, int h_size, int v_size, int max_SLA, int * K, int ** network_utilization, int l_size, int ** G, int ** T);
void repair_population(int ** population, int *** utilization, int ** H, int ** V, int number_of_individuals, int h_size, int v_size, int max_SLA, int * K, int ** network_utilization, int l_size, int ** G, int ** T);
void repair_individual(int ** population, int *** utilization, int ** H, int ** V, int number_of_individuals, int h_size, int v_size, int max_SLA, int * K, int ** network_utilization, int l_size, int individual, int ** G, int ** T);
int is_overloaded(int **H, int ***utilization, int individual, int physical);
int is_overassigned(int *K, int **network_utilization, int individual, int link);
