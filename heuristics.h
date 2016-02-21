/* 
 * heuristics.h: Virtual Machine Placement Problem - Heuristics Functions Header
 * Date: 21-02-2016
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */

/* include libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "print_functions.h"
#include "scenario.h"
 
/* definitions */


/* get the number of physical and virtual machines */
int first_fit(float *S, float **utilization, int **placement, int **H, int h_size);

bool checkResources(float *request, float *utilization, int *H, int h_size);