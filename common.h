/* 
 * common.h: Virtual Machine Placement Problem - Common Functions
 * Date: February, 2016
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Author: Rodrigo Ferreira (rodrigofepy@gmail.com)
 * Author: Saúl Zalimben (szalimben93@gmail.com)
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


/* function headers definitions */

/* get the number of physical and virtual machines */
int get_h_size(char path_to_file[]);

/* load datacenter physical resources and virtual requirements */
int** load_H(int h_size, char path_to_file[]);

/* return the number of unique VM */
int number_unique_vm (float **S, int s_size);


