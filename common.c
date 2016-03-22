/* 
 * common.c: Virtual Machine Placement Problem - Common Functions
 * Date: February, 2016
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Author: Rodrigo Ferreira (rodrigofepy@gmail.com)
 * Author: Saúl Zalimben (szalimben93@gmail.com)
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
			/* if the line is equal to S_HEADER, we end the physical machines block in the file */
			if (strstr(input_line,S_HEADER) != NULL) {
				reading_physical = 0;
				break;
			}
			/* if it is the correct block in the file, it is not the header and it is not a blank line or carriage return (ascii 13), we count */
			if (reading_physical == 1 && strstr(input_line,H_HEADER) == NULL && strcmp(input_line, "\n") != 0 
				&& input_line[0] != 13) {
				h_size++;
			}
		}
	}
	/* close the file */
	fclose(datacenter_file);
	/* return the value */
	return h_size;
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
			if (strstr(input_line,S_HEADER) != NULL) {
				reading_physical = 0;
				break;
			}
			/* if it's the correct block in the file, it is not the header and it is not a blank line or carriage return (ascii 13), we count */
			if (reading_physical == 1 && strstr(input_line,H_HEADER) == NULL && strcmp(input_line, "\n") != 0 
				&& input_line[0] != 13) {
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

/**
 * number_unique_vm: return the number of unique VM 
 * parameter: S matrix
 * return: number of unique VM
 */
int number_unique_vm (float **S, int s_size) {	
	int number_unique_vm = 0;
	int iterator_row;
	for (iterator_row = 0; iterator_row < s_size; ++iterator_row) {
 		if(S[iterator_row][0] <= S[iterator_row][12]) {
			number_unique_vm++;
		}
 	}
 	return number_unique_vm;
}
