/* 
 * common.c: Virtual Machine Placement Problem - Common Functions
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */
	
/* include common header */
#include "common.h"

/* get_h_size: returns the number of physical machines
 * parameter: path to the datacenter file
 * returns: number of physical machines
 */
int get_h_size(char path_to_file[])
{
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
	if (datacenter_file != NULL) 
	{ 
		/* read until the end */
		while(!feof(datacenter_file)) 
		{
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is equal to H_HEADER, we begin the physical machines block in the file */
			if (strstr(input_line,H_HEADER) != NULL)
			{
				reading_physical = 1;
			}
			/* if the line is equal to V_HEADER, we end the physical machines block in the file */
			if (strstr(input_line,V_HEADER) != NULL) 
			{
				reading_physical = 0;
				break;
			}
			/* if it is the correct block in the file, it is not the header and it is not a blank line, we count */			
			if (reading_physical == 1 && strstr(input_line,H_HEADER) == NULL && strcmp(input_line, "\n") != 0)
			{
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
int get_v_size(char path_to_file[])
{
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
	if (datacenter_file != NULL) 
	{ 
		/* read until the end */
		while(!feof(datacenter_file)) 
		{
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is equal to V_HEADER, we begin the virtual machines block in the file */
			if (strstr(input_line,V_HEADER) != NULL)
			{
				reading_virtual = 1;
			}
			/* if the line is equal to T_HEADER, we end the virtual machines block in the file */
			if (strstr(input_line,T_HEADER) != NULL)
			{
				reading_virtual = 0;
				break;
			}
			/* if we are in the correct block in the file, it is not the header and it is not a blank line, we count */
			if (reading_virtual == 1 && strstr(input_line,V_HEADER) == NULL && strcmp(input_line, "\n") != 0)
			{
				v_size++;
			}
		}
	}
	/* close the file */
	fclose(datacenter_file);
	/* return the value */
	return v_size;
}

/* print_int_matrix: prints on screen a integer matrix
 * parameter: matrix to print
 * parameter: number of individuals
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_int_matrix(int **matrix, int rows, int columns)
{
	/* iterators */
	int iterator_row;
	int iterator_column;
	/* iterate on rows */	
	for (iterator_row=0; iterator_row < rows; iterator_row++)
	{
		printf("[DEBUG] ROW %d:\t\t",iterator_row);	
		/* iterate on columns */
		for (iterator_column = 0; iterator_column < columns; iterator_column++)
		{
			printf("%d ",matrix[iterator_row][iterator_column]);	
		}
		printf("\n");
	}
}

/* print_float_matrix: prints on screen a float matrix
 * parameter: matrix to print
 * parameter: number of individuals
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_float_matrix(float **matrix, int rows, int columns)
{
	/* iterators */
	int iterator_row;
	int iterator_column;
	/* iterate on rows */	
	for (iterator_row=0; iterator_row < rows; iterator_row++)
	{
		printf("[DEBUG] ROW %d:\t",iterator_row);	
		/* iterate on columns */
		for (iterator_column = 0; iterator_column < columns; iterator_column++)
		{
			printf("%g\t",matrix[iterator_row][iterator_column]);	
		}
		printf("\n");
	}
}

/* print_int_array: prints on screen a int array
 * parameter: array to print
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_int_array(int *array, int columns)
{
	/* iterators */
	int iterator_column;
	/* iterate on columns */
	for (iterator_column = 0; iterator_column < columns; iterator_column++)
	{
		printf("[DEBUG] [%d]: %d\n",iterator_column,array[iterator_column]);	
	}
}

/* print_float_array: prints on screen a float array
 * parameter: array to print
 * parameter: number of columns
 * returns: nothing, it's void
 */
void print_float_array(float *array, int columns)
{
	/* iterators */
	int iterator_column;
	/* iterate on columns */
	for (iterator_column = 0; iterator_column < columns; iterator_column++)
	{
		printf("[DEBUG] [%d]: %g\n",iterator_column,array[iterator_column]);	
	}
}

/* load_H: load the values of H
 * parameter: number of physical machines
 * parameter: path to the datacenter file
 * returns: H matrix
 */
int** load_H(int h_size, char path_to_file[])
{
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
	if (datacenter_file != NULL) 
	{ 
		/* read until the end */
		while(!feof(datacenter_file)) 
		{
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is equal to H_HEADER, we begin the physical machines block in the file */
			if (strstr(input_line,H_HEADER) != NULL)
			{
				reading_physical = 1;
			}
			/* if the line is equal to V_HEADER, we end the physical machines block in the file */
			if (strstr(input_line,V_HEADER) != NULL) 
			{
				reading_physical = 0;
				break;
			}
			/* if it's the correct block in the file, it is not the header and it is not a blank line, we count */			
			if (reading_physical == 1 && strstr(input_line,H_HEADER) == NULL && strcmp(input_line, "\n") != 0)
			{
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
int** load_V(int v_size, char path_to_file[])
{
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
	if (datacenter_file != NULL) 
	{ 
		/* read until the end */
		while(!feof(datacenter_file)) 
		{
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is equal to V_HEADER, we begin the virtual machines block in the file */
			if (strstr(input_line,V_HEADER) != NULL)
			{
				reading_virtual = 1;
			}
			/* if the line is equal to T_HEADER, we end the virtual machines block in the file */
			if (strstr(input_line,T_HEADER) != NULL) 
			{
				reading_virtual = 0;
				break;
			}
			/* if it's the correct block in the file, it is not the header and it is not a blank line, we count */			
			if (reading_virtual == 1 && strstr(input_line,V_HEADER) == NULL && strcmp(input_line, "\n") != 0)
			{
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

/* load_utilization: loads the utilization of the physical machines of all the individuals
 * parameter: population matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * returns: utilization tridimentional matrix
 */
int*** load_utilization(int **population, int **H, int **V, int number_of_individuals, int h_size, int v_size)
{
	/* iterators */
	int iterator_individual;
	int iterator_virtual;
	int iterator_physical;
	/* utilization holds the physical machines utilization of Processor, Memory and Storage of every individual */
	int ***utilization = (int ***) malloc (number_of_individuals *sizeof (int **));
	/* iterate on individuals */	
	for (iterator_individual=0; iterator_individual < number_of_individuals; iterator_individual++)
	{
		/* requirements matrix, holds the sum of virtual machines requirements for each physical machine */
		int **requirements = (int **) malloc (h_size *sizeof (int *));
		/* utilization holds the physical machines utilization of Processor, Memory and Storage of every individual */
		utilization[iterator_individual] = (int **) malloc (h_size *sizeof (int *));
		for (iterator_physical=0; iterator_physical < h_size; iterator_physical++)
		{
			/* virtual machine requirements in Processor, Memory and Storage. Initialized to 0 */
			requirements[iterator_physical] = (int *) malloc (3 *sizeof (int));
 			requirements[iterator_physical][0] = requirements[iterator_physical][1] = requirements[iterator_physical][2] = 0;
			/* physical machine utilization of Processor, Memory and Storage. Initialized to 0 */
			utilization[iterator_individual][iterator_physical] = (int *) malloc (3 *sizeof (int));
			utilization[iterator_individual][iterator_physical][0] = utilization[iterator_individual][iterator_physical][1] = 
			utilization[iterator_individual][iterator_physical][2] = 0;
		}
		/* iterate on positions of an individual */
		for (iterator_virtual = 0; iterator_virtual < v_size; iterator_virtual++)
		{
			/* if the virtual machine has a placement assigned */
			if (population[iterator_individual][iterator_virtual] != 0)
			{
				/* increment the requirements of the assigned physical machine with the virtual machine requirements
				of Processor, Memory and Storage */
				requirements[population[iterator_individual][iterator_virtual]-1][0] += V[iterator_virtual][0];
				requirements[population[iterator_individual][iterator_virtual]-1][1] += V[iterator_virtual][1];
				requirements[population[iterator_individual][iterator_virtual]-1][2] += V[iterator_virtual][2];
			}
		}
		/* iterate on positions of an individual */
		for (iterator_physical=0; iterator_physical < h_size; iterator_physical++)
		{
			/* virtual machine requirements in Processor, Memory and Storage. Initialized to 0 */
			utilization[iterator_individual][iterator_physical][0] = requirements[iterator_physical][0];
			utilization[iterator_individual][iterator_physical][1] = requirements[iterator_physical][1];
			utilization[iterator_individual][iterator_physical][2] = requirements[iterator_physical][2];
		}
	}
	return utilization;
}

/* load_objectives: calculate the cost of each objective of each solution
 * parameter: population matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: network traffic matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * returns: cost of each objetive matrix
 */
float** load_objectives(int **population, int ***utilization, int **H, int **V, int **T, int number_of_individuals, int h_size, int v_size, int *K, int **network_utilization, int l_size)
{
	/* iterators */
	int iterator_individual;
	int iterator_virtual2;
	int iterator_virtual;
	int physical_position;
	int iterator_physical;
	int iterator_link;
	float power_consumption;
	float MLU = -1.0;
	float n_u = 0.0;
	float k = 0.0;
	/* count the network traffic */
	int network_traffic;
	int count_traffic;
	/* utility of a physical machine */
	float utilidad;
	/* value solution holds the cost of each solution */
	float **value_solution = (float **) malloc (number_of_individuals *sizeof (float *));
	for (iterator_individual = 0 ; iterator_individual < number_of_individuals; iterator_individual++)
		value_solution[iterator_individual] = (float *) malloc (5 *sizeof (float));
	/* iterate on individuals */
	for (iterator_individual = 0; iterator_individual < number_of_individuals; iterator_individual++)
	{
		power_consumption = 0.0;
		value_solution[iterator_individual][0] = value_solution[iterator_individual][1] = value_solution[iterator_individual][2] = 
		value_solution[iterator_individual][3] = value_solution[iterator_individual][4] = 0.0;

		/* (OF1) calculate energy consumption of each solution*/
		/* iterate on physical machines */
		for (iterator_physical = 0 ; iterator_physical < h_size ; iterator_physical++)
		{
			if (utilization[iterator_individual][iterator_physical][0] > 0)
			{
				/* calculates utility of a physical machine */
				utilidad = (float)utilization[iterator_individual][iterator_physical][0] / H[iterator_physical][0];
				/* calculates energy consumption of a physical machine */
				power_consumption += ((float)H[iterator_physical][3] - ((float)H[iterator_physical][3]*0.01)) * utilidad + 
				(float)H[iterator_physical][3]*0.6;
			}
		}
		/* loads energy consumption of each solution */
		value_solution[iterator_individual][0] = power_consumption;

		/* (OF2) calculate network traffic between virtual machines */
		network_traffic = 0;
		/* iterate on virtual machines */
		for (iterator_virtual = 0; iterator_virtual < v_size; iterator_virtual++) 
		{
			physical_position = population[iterator_individual][iterator_virtual];
			/* count the network traffic */
			count_traffic = 0;
			if (population[iterator_individual][iterator_virtual] != 0)
			{
				for (iterator_virtual2 = 0 ; iterator_virtual2 < v_size ; iterator_virtual2++) 
				{
					if (physical_position == population[iterator_individual][iterator_virtual2] || 
						population[iterator_individual][iterator_virtual2] == 0) 
					{
						count_traffic = 0;
					} 
					else 
					{
						count_traffic = T[iterator_virtual][iterator_virtual2];
					}
					/* calculates network traffic between all virtual machines of each solution */
					network_traffic = network_traffic + count_traffic;
				}
			}
		}
		/* loads the network traffic of each solution */
		value_solution[iterator_individual][1] = (float)network_traffic;

		/* (OF3) calculate revenue of each solution */
		/* (OF4) calculate QoS of each solution */
		/* iterate on virtual machines */
		for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
		{
			physical_position = population[iterator_individual][iterator_virtual];
			if (physical_position > 0)
			{
				/* loads the revenue of each solution */
				value_solution[iterator_individual][2] += (float) V[iterator_virtual][4];
				/* loads the QoS of each solution */
				value_solution[iterator_individual][3] += (float) pow (CONSTANT,V[iterator_virtual][3]) * V[iterator_virtual][3];
			}
		}
		/* (OF5) calculate MLU of each solution */
		/* iterate on network link */
		for (iterator_link = 0 ; iterator_link < l_size ; iterator_link++)
		{
			n_u = network_utilization[iterator_individual][iterator_link] / 1.0f;
			k = K[iterator_link] / 1.0f;
			if ((float) n_u / k > MLU)
			{
				/* loads the MLU */
				MLU = n_u / k;
			}
		}
		/* loads the MLU of each solution */
		value_solution[iterator_individual][4] = MLU;
		MLU = -1.0;
		n_u = 0.0;
		k = 0.0;
	}
	return value_solution;
}
