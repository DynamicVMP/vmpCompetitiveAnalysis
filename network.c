/* 
 * network.c: Virtual Machine Placement Problem - Network Model Functions
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */
	
/* include network header */
#include "network.h"
#include "scenario.h"

/* get_l_size: returns the number of network links
 * parameter: path to the datacenter file
 * returns: number of network links
 */
int get_l_size(char path_to_file[]) {
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* number of network links */
	int l_size = 0;
	/* 1 if is reading the network topology block in the file */
	int reading_links = 0;
 	/* open the file for reading */
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */
	if (datacenter_file != NULL) { 
		/* read until the end */
		while(!feof(datacenter_file)) {
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is equal to L_HEADER, we begin the network topology block in the file */
			if (strstr(input_line,L_HEADER) != NULL) {
				reading_links = 1;
			}
			/* if the line is equal to C_HEADER, we end the network topology block in the file */
			if (strstr(input_line,C_HEADER) != NULL) {
				reading_links = 0;
				break;
			}
			/* if we are in the correct block in the file, it is not the header and it is not a blank line or carriage return (ascii 13), we count */
			if (reading_links == 1 && strstr(input_line,L_HEADER) == NULL && strcmp(input_line, "\n") != 0 
				&& input_line[0] != 13) {
				l_size++;
			}
		}
	}
	/* close the file */
	fclose(datacenter_file);
	/* return the value */
	return l_size;
}

/* load_T: load the values of T
 * parameter: number of virtual machines
 * parameter: path to the datacenter file
 * returns: T matrix
 */
int** load_T(int v_size, char path_to_file[]) {
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* iterators */
	int iterator_row = 0;
	int iterator_column = 0;
	int iterator_line = 0;
	/* 1 if is reading the network traffic block in the file */
	int reading_traffic = 0;
	char *ptr;
	/* memory reserve for v_size virtual machines */
	int **T = (int **) malloc (v_size *sizeof (int *));
 	/* open the file for reading */
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */    
	if (datacenter_file != NULL) { 
		/* read until the end */
		while(iterator_row < v_size) {
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			iterator_line++;
			/* reset iterator of columns */
			iterator_column = 0;
			/* if the line is equal to T_HEADER, we begin the network traffic block in the file */
			if (strstr(input_line,T_HEADER) != NULL) {
				reading_traffic = 1;
			}
			/* if it's the correct block in the file, it is not the header and it is not a blank line or carriage return (ascii 13), we count */
			if (reading_traffic == 1 && strstr(input_line,T_HEADER) == NULL && strcmp(input_line, "\n") != 0 
				&& input_line[0] != 13) {
				/* memory reserve for v_size virtual machines */
				T[iterator_row] = (int *) malloc (v_size *sizeof (int));
				/* strtok first time and load the value */
				ptr = strtok(input_line," \n\t");
				sscanf(ptr,"%d",&T[iterator_row][iterator_column]);
				/* load all the next tokens of the input line */
				while( (ptr = strtok( NULL, "\t" )) != NULL ) {		
					/* increment iterator of columns and load the value */
					iterator_column++;
					sscanf(ptr,"%d",&T[iterator_row][iterator_column]);
				}
				/* increment iterator of rows */
				iterator_row++;
			} 
		}
	}
	fclose(datacenter_file);
	return T;
}

/* load_G: load the values of G
 * parameter: number of physical machines
 * parameter: number of network links
 * parameter: path to the datacenter file
 * returns: G matrix
 */
int** load_G(int h_size, int l_size, char path_to_file[]) {
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* iterators */
	int iterator_row = 0;
	int iterator_column = 0;
	int iterator_line = 0;
	/* 1 if is reading the network topology block in the file */
	int reading_topology = 0;
	char *ptr;
	/* memory reserve for l_size network links */
	int **G = (int **) malloc (l_size *sizeof (int *));
 	/* open the file for reading */
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */    
	if (datacenter_file != NULL) { 
		/* read until the end */
		while(iterator_row < l_size) {
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			iterator_line++;
			/* reset iterator of columns */
			iterator_column = 0;
			/* if the line is equal to L_HEADER, we begin the network topology block in the file */
			if (strstr(input_line,L_HEADER) != NULL) {
				reading_topology = 1;
			}
			/* if it's the correct block in the file, it is not the header and it is not a blank line or carriage return (ascii 13), we count */
			if (reading_topology == 1 && strstr(input_line,L_HEADER) == NULL && strcmp(input_line, "\n") != 0 
				&& input_line[0] != 13) {
				/* memory reserve for h_size physical machines */
				G[iterator_row] = (int *) malloc (h_size *sizeof (int));
				/* strtok first time and load the value */
				ptr = strtok(input_line," \n\t");
				sscanf(ptr,"%d",&G[iterator_row][iterator_column]);
				/* load all the next tokens of the input line */
				while( (ptr = strtok( NULL, "\t" )) != NULL ) {		
					/* increment iterator of columns and load the value */
					iterator_column++;
					sscanf(ptr,"%d",&G[iterator_row][iterator_column]);
				}
				/* increment iterator of rows */
				iterator_row++;
			} 
		}
	}
	fclose(datacenter_file);
	return G;
}

/* load_K: load the values of K
 * parameter: number of network links
 * parameter: path to the datacenter file
 * returns: K array
 */
int* load_K(int l_size, char path_to_file[]) {
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* iterators */
	int iterator_row = 0;
	int iterator_line = 0;
	/* 1 if is reading the network link capacity block in the file */
	int reading_capacity = 0;
	char *ptr;
	/* memory reserve for l_size network links */
	int *K = (int *) malloc (l_size *sizeof (int));
 	/* open the file for reading */
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */    
	if (datacenter_file != NULL) { 
		/* read until the end */
		while(iterator_row < l_size)  {
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			iterator_line++;
			/* if the line is equal to C_HEADER, we begin the network link capacity block in the file */
			if (strstr(input_line,C_HEADER) != NULL) {
				reading_capacity = 1;
			}
			/* if it's the correct block in the file, it is not the header and it is not a blank line or carriage return (ascii 13), we count */
			if (reading_capacity == 1 && strstr(input_line,C_HEADER) == NULL && strcmp(input_line, "\n") != 0 
				&& input_line[0] != 13) {
				/* strtok first time and load the value */
				ptr = strtok(input_line," \n\t");
				sscanf(ptr,"%d",&K[iterator_row]);
				/* load all the next tokens of the input line */
				/* increment iterator of rows */
				iterator_row++;
			} 
		}
	}
	fclose(datacenter_file);
	return K;
}

/* load_network_utilization: loads the utilization of the network links of all the individuals
 * parameter: population matrix
 * parameter: network topology matrix
 * parameter: network link capacity array
 * parameter: number of individuals
 * parameter: number of network links
 * parameter: number of virtual machines
 * returns: network utilization tridimentional matrix
 */
int** load_network_utilization(int **population, int **G, int **T, int number_of_individuals, int l_size, int v_size) {
	/* iterators */
	int iterator_individual;
	int iterator_virtual;
	int iterator_virtual_again;
	int iterator_link;
	/* utilization holds the utilization of network links of each individual */
	int **network_utilization = (int **) malloc (number_of_individuals *sizeof (int *));
	/* iterate on individuals */	
	for (iterator_individual=0; iterator_individual < number_of_individuals; iterator_individual++) {
		/* network utilization matrix, holds the utilization of network links per individual */
		network_utilization[iterator_individual] = (int *) malloc (l_size *sizeof (int));
		for (iterator_link=0; iterator_link < l_size; iterator_link++) {
			/* initialize link value */
			network_utilization[iterator_individual][iterator_link] = 0;
		}
		/* for each virtual machine in T, add the corresponding traffic to other virtual machines */
		for (iterator_virtual=0; iterator_virtual < v_size; iterator_virtual++) {
			for (iterator_virtual_again=0; iterator_virtual_again < v_size; iterator_virtual_again++) {
				/* consider only traffic between virtual machines in different physical machines for placed only virtual machines */
				if (population[iterator_individual][iterator_virtual] != population[iterator_individual][iterator_virtual_again] && 				
					population[iterator_individual][iterator_virtual] != 0 && population[iterator_individual][iterator_virtual_again] != 0) {
					/* for each network link */
					for (iterator_link=0; iterator_link < l_size; iterator_link++) {
						/* add the corresponding traffic for each used link */
						if (G[population[iterator_individual][iterator_virtual]-1][iterator_link] !=0 || 
							G[population[iterator_individual][iterator_virtual_again]-1][iterator_link] !=0) {						
							network_utilization[iterator_individual][iterator_link] += T[iterator_virtual][iterator_virtual_again];
						}
					}
				}
			}
		}			
	}
	return network_utilization;
}
