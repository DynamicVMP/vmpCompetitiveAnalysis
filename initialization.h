/* 
 * initialization.c: Virtual Machine Placement Problem - Initialization Stage
 * Date: February, 2016
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */

/* include libraries */
#include "common.h"
 
/* function headers definitions */
float** utilization_initialization(int h_size, int resources);
int** placement_initialization(int h_size, int v_size);
 