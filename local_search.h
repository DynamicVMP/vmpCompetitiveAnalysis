/*
 * local_search.h: Virtual Machine Placement Problem - Local Search Stage Header
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include libraries */
#include <stdio.h>
#include <stdlib.h>

/* function headers definitions */
int** local_search(int **population, int ***utilization, int **H, float **V, int number_of_individuals, int h_size, int v_size);
