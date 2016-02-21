/* 
 * network.h: Virtual Machine Placement Problem - Common Functions Header
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
/* definitions */
#define T_HEADER "NETWORK TRAFFIC"
#define L_HEADER "NETWORK TOPOLOGY"
#define C_HEADER "NETWORK LINK CAPACITY"
#define TAM_BUFFER BUFSIZ
/* get the number of network links */
int get_l_size(char path_to_file[]);
/* load datacenter network topology */
int** load_T(int v_size, char path_to_file[]);
int** load_G(int h_size, int l_size, char path_to_file[]);
int* load_K(int l_size, char path_to_file[]);
/* load utilization of network resources */
int** load_network_utilization(int **population, int **G, int **T, int number_of_individuals, int l_size, int v_size);
