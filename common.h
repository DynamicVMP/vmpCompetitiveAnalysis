/* 
 * common.h: Virtual Machine Placement Problem - Common Functions Header
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include libraries */
#include <stdbool.h>
#include "utils.h"
 
/* definitions */
#define H_HEADER "PHYSICAL MACHINES"
#define V_HEADER "VIRTUAL MACHINES"
#define T_HEADER "NETWORK TRAFFIC"
#define S_HEADER "SCENARIOS"

#define TAM_BUFFER BUFSIZ
#define CONSTANT 10000
#define BIG_COST 1000000

/* get the number of physical and virtual machines */
int get_h_size(char path_to_file[]);
/* load datacenter physical resources and virtual requirements */
int** load_H(int h_size, char path_to_file[]);

/* objective functions */
float power_consumption (float **utilization, int **H, int h_size);

