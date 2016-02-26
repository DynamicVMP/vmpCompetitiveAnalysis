/* 
 * initialization.h: Virtual Machine Placement Problem - Initialization Stage Header
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include libraries */
#include "common.h"
 
/* function headers definitions */
float** utilization_initialization(int h_size, int resources);
int** placement_initialization(int h_size, int v_size);
