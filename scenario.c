/* 
 * scenario.c: Virtual Machine Placement Problem - Scenario Load
 * Date: February, 2016
 * Author: Saúl Zalimben
 */

#include "scenario.h"

int get_s_size(char path_to_file[]) {

    /* scenario file to read from */
    FILE *scenario_file;
    /* line readed from file */
    char input_line[TAM_BUFFER];
    /* number of scenario */
    int s_size = 0;
    /* 1 if is reading the scenario block in the file */
    int reading_scenario = 0;
    /* open the file for reading */ 
    scenario_file = fopen(path_to_file,"r");
    
    /* if it is opened ok, we continue */    
    if (scenario_file != NULL) { 
        /* read until the end */
        while(!feof(scenario_file)) {
            /* get line per line */
            fgets(input_line, TAM_BUFFER, scenario_file);
            /* if the line is equal to S_HEADER, we begin the scenario block in the file */
            if (strstr(input_line,S_HEADER) != NULL) {
                reading_scenario = 1;
            }
            /* if it is the correct block in the file, it is not the header and it is not a blank line, we count */         
            if (reading_scenario == 1 && strstr(input_line,S_HEADER) == NULL && strcmp(input_line, "\n") != 0) {
                s_size++;
            }
        }
    }
    /* close the file */
    fclose(scenario_file);
    /* return the value */
    return s_size;
}

float** load_S(int s_size, char path_to_file[]) {

    FILE *scenario_file;
    char input_line[10000];
    int iterator = 0;
    int reading_scenario = 0;
    float **S = (float **) malloc (s_size *sizeof (float *));
    scenario_file = fopen(path_to_file,"r");
    
    if (scenario_file != NULL) { 
        while(!feof(scenario_file)) {
            fgets(input_line, TAM_BUFFER, scenario_file);
            if (strstr(input_line,S_HEADER) != NULL) {
                reading_scenario = 1;
            }
            
            if (reading_scenario == 1 && strstr(input_line,S_HEADER) == NULL && strcmp(input_line, "\n") != 0) {
                S[iterator] = (float *) malloc (14 *sizeof (float));
                sscanf(input_line,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",&S[iterator][0],&S[iterator][1],&S[iterator][2],&S[iterator][3],&S[iterator][4],&S[iterator][5],&S[iterator][6],&S[iterator][7],&S[iterator][8],&S[iterator][9],&S[iterator][10],&S[iterator][11],&S[iterator][12],&S[iterator][13]);
                iterator++;
            }
        }
    }
    fclose(scenario_file);
    return S;
}


int main (int arg, char *argv[]) {

	char file[] = "/home/szalimben/tesis/iMaVMP/inputs/SCENARIOS";

	int s_size = get_s_size(file);

	float ** pepe = load_S(s_size, file);

	// print_int_matrix(pepe, 2, 14);
	print_float_matrix(pepe, 2, 14);

}

void print_float_matrix(float **matrix, int rows, int columns) {
	/* iterators */
	int iterator_row;
	int iterator_column;
	/* iterate on rows */	
	for (iterator_row=0; iterator_row < rows; iterator_row++) {
		printf("[DEBUG] ROW %d:\t",iterator_row);	
		/* iterate on columns */
		for (iterator_column = 0; iterator_column < columns; iterator_column++) {
			printf("%g\t",matrix[iterator_row][iterator_column]);	
		}
		printf("\n");
	}
}

void print_int_matrix(int **matrix, int rows, int columns) {
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