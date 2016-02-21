/* 
 * common.c: Virtual Machine Placement Problem - Common Functions
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */
	
/* include common header */
#include "common.h"
#include "print_functions.h"

/* get_h_size: returns the number of physical machines
 * parameter: path to the datacenter file
 * returns: number of physical machines
 */
int get_h_size(char path_to_file[]) {
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* number of physical machines */
	int h_size = 0;
	/* 1 if reading the physical machines block in the file */
	int reading_physical = 0;
 	/* open the file for reading */ 
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */    
	if (datacenter_file != NULL) { 
		/* read until the end */
		while(!feof(datacenter_file)) {
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is equal to H_HEADER, we begin the physical machines block in the file */
			if (strstr(input_line,H_HEADER) != NULL) {
				reading_physical = 1;
			}
			/* if the line is equal to V_HEADER, we end the physical machines block in the file */
			if (strstr(input_line,V_HEADER) != NULL) {
				reading_physical = 0;
				break;
			}
			/* if it is the correct block in the file, it is not the header and it is not a blank line, we count */			
			if (reading_physical == 1 && strstr(input_line,H_HEADER) == NULL && strcmp(input_line, "\n") != 0) {
				h_size++;
			}
		}
	}
	/* close the file */
	fclose(datacenter_file);
	/* return the value */
	return h_size;
}

/* get_v_size: returns the number of virtual machines
 * parameter: path to the datacenter file
 * returns: number of virtual machines
 */
int get_v_size(char path_to_file[]) {
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* number of virtual machines */
	int v_size = 0;
	/* 1 if reading the virtual machines block in the file */
	int reading_virtual = 0;
 	/* open the file for reading */
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */
	if (datacenter_file != NULL) { 
		/* read until the end */
		while(!feof(datacenter_file)) {
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is equal to V_HEADER, we begin the virtual machines block in the file */
			if (strstr(input_line,V_HEADER) != NULL) {
				reading_virtual = 1;
			}
			/* if the line is equal to T_HEADER, we end the virtual machines block in the file */
			if (strstr(input_line,T_HEADER) != NULL) {
				reading_virtual = 0;
				break;
			}
			/* if we are in the correct block in the file, it is not the header and it is not a blank line, we count */
			if (reading_virtual == 1 && strstr(input_line,V_HEADER) == NULL && strcmp(input_line, "\n") != 0) {
				v_size++;
			}
		}
	}
	/* close the file */
	fclose(datacenter_file);
	/* return the value */
	return v_size;
}


/* load_H: load the values of H
 * parameter: number of physical machines
 * parameter: path to the datacenter file
 * returns: H matrix
 */
int** load_H(int h_size, char path_to_file[]) {
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* iterator */
	int iterator = 0;
	/* 1 if is reading the physical machines block in the file */
	int reading_physical = 0;
	/* memory reserve for h_size physical machines */
	int **H = (int **) malloc (h_size *sizeof (int *));
 	/* open the file for reading */ 
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */    
	if (datacenter_file != NULL) { 
		/* read until the end */
		while(!feof(datacenter_file)) {
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is equal to H_HEADER, we begin the physical machines block in the file */
			if (strstr(input_line,H_HEADER) != NULL) {
				reading_physical = 1;
			}
			/* if the line is equal to V_HEADER, we end the physical machines block in the file */
			if (strstr(input_line,V_HEADER) != NULL) {
				reading_physical = 0;
				break;
			}
			/* if it's the correct block in the file, it is not the header and it is not a blank line, we count */			
			if (reading_physical == 1 && strstr(input_line,H_HEADER) == NULL && strcmp(input_line, "\n") != 0) {
				/* reserve 4 columns for Processor, Memory, Storage and Power Consumption */
				H[iterator] = (int *) malloc (4 *sizeof (int));
				/* load on the matrix and increment iterator */
				sscanf(input_line,"%d %d %d %d\n",&H[iterator][0],&H[iterator][1],&H[iterator][2],&H[iterator][3]);
				iterator++;
			}
		}
	}
	fclose(datacenter_file);
	return H;
}

/* load_V: load the values of V
 * parameter: number of virtual machines
 * parameter: path to the datacenter file
 * returns: V matrix
 */
int** load_V(int v_size, char path_to_file[]) {
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* iterator */
	int iterator = 0;
	/* 1 if is reading the virtual machines block in the file */
	int reading_virtual = 0;
	/* memory reserve for v_size virtual machines */
	int **V = (int **) malloc (v_size *sizeof (int *));
 	/* open the file for reading */ 
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */    
	if (datacenter_file != NULL) { 
		/* read until the end */
		while(!feof(datacenter_file)) {
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is equal to V_HEADER, we begin the virtual machines block in the file */
			if (strstr(input_line,V_HEADER) != NULL) {
				reading_virtual = 1;
			}
			/* if the line is equal to T_HEADER, we end the virtual machines block in the file */
			if (strstr(input_line,T_HEADER) != NULL) {
				reading_virtual = 0;
				break;
			}
			/* if it's the correct block in the file, it is not the header and it is not a blank line, we count */			
			if (reading_virtual == 1 && strstr(input_line,V_HEADER) == NULL && strcmp(input_line, "\n") != 0) {
				/* reserve 5 columns for Processor, Memory, Storage and Economical Revenue */
				V[iterator] = (int *) malloc (5 *sizeof (int));
				sscanf(input_line,"%d %d %d %d %d\n",&V[iterator][0],&V[iterator][1],&V[iterator][2],&V[iterator][3],&V[iterator][4]);
				iterator++;
			}
		}
	}
	fclose(datacenter_file);
	return V;
}
