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


/* get_v_per_t: counts the number virtual machines requests
 * parameter: the scenario matrix
 * parameter: the time t considered
 * parameter: the size of the scenario
 * returns: number of unique virtual machines requests
 */
int get_v_size_per_t(float** matrix_s, int t, int max_row){

    int v_size = 0;
    int iterator = 0;

    while(iterator<max_row && matrix_s[iterator][0]<=t){
        /* - This condition is used to consider only new vm request
           - if a virtual machine has the init time(matrix_s[iterator][12]) equal
               to the t_actual(matrix_s[iterator][0]) where t_actual<=t */
        if(matrix_s[iterator][12]==matrix_s[iterator][0]){
            v_size++;
        }
        iterator++;
    }

    return v_size;
}

/* load_v_per_t: loads virtual machines requests
 * parameter: the scenario matrix
 * parameter: the size of the scenario
 * parameter: number of virtual machines
 * parameter: the time t considered
 * returns: the matrix V of virtual machines
 */
float ** load_v_per_t(float ** matrix_s,int s_size,int v_size, int t){

    int iterator;
    float **matrix_v = (float**)malloc(v_size*sizeof(float*));
    for(iterator=0;iterator<v_size;iterator++){
        matrix_v[iterator] = (float *) malloc(10 * sizeof(float));
    }

    int iterator_v= 0;
    iterator = 0;
    while(iterator<s_size && matrix_s[iterator][0]<=t){

        /* - This condition is used to consider only new vm request
          - if a virtual machine has the init time(matrix_s[iterator][12]) equal
              to the t_actual(matrix_s[iterator][0]) where t_actual<=t */
        if(matrix_s[iterator][12]==matrix_s[iterator][0]){

            /*This condition checks if a virtual machine will be placed and destroyed in the range 0-t.*/
            if(matrix_s[iterator][13]<t){
                matrix_v[iterator_v][0] = 0;
                matrix_v[iterator_v][1] = 0;
                matrix_v[iterator_v][2] = 0;
                matrix_v[iterator_v][3] = 0;
                matrix_v[iterator_v][4] = 0;
                matrix_v[iterator_v][5] = 0;
                matrix_v[iterator_v][6] = 0;
                matrix_v[iterator_v][7] = 0;
                matrix_v[iterator_v][8] = 0;
                matrix_v[iterator_v][9] = 0;

            }else {
                matrix_v[iterator_v][0] = matrix_s[iterator][4]; //cpu
                matrix_v[iterator_v][1] = matrix_s[iterator][5]; //ram
                matrix_v[iterator_v][2] = matrix_s[iterator][6]; //net
                matrix_v[iterator_v][3] = matrix_s[iterator][11]; //SLA
                matrix_v[iterator_v][4] = matrix_s[iterator][10]; //revenue
                matrix_v[iterator_v][5] = matrix_s[iterator][12]; //t_init
                matrix_v[iterator_v][6] = matrix_s[iterator][13]; //t_end
                matrix_v[iterator_v][7] = matrix_s[iterator][1]; //Sb
                matrix_v[iterator_v][8] = matrix_s[iterator][2]; //DCc
                matrix_v[iterator_v][9] = matrix_s[iterator][3]; //Vj

            }
            iterator_v++;
        }
        iterator++;
    }

    return matrix_v;
}


/* load_utilization: loads the utilization of the physical machines of all the individuals
 * parameter: utilization matrix
 * parameter: population matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * returns: utilization tridimentional matrix
 */
int*** load_utilization(int*** utilization, int **population, int **H, float **V, int number_of_individuals, int h_size, int v_size)
{
    /* iterators */
    int iterator_individual;
    int iterator_virtual;
    int iterator_physical;

    /* utilization holds the physical machines utilization of Processor, Memory and Storage of every individual */
    /* iterate on individuals */
    for (iterator_individual=0; iterator_individual < number_of_individuals; iterator_individual++)
    {
        /* requirements matrix, holds the sum of virtual machines requirements for each physical machine */
        int **requirements = (int **) malloc (h_size *sizeof (int *));
        for (iterator_physical=0; iterator_physical < h_size; iterator_physical++)
        {
            /* virtual machine requirements in Processor, Memory and Storage. Initialized to 0 */
            requirements[iterator_physical] = (int *) malloc (3 *sizeof (int));
            requirements[iterator_physical][0] = requirements[iterator_physical][1] = requirements[iterator_physical][2] = 0;
            /* physical machine utilization of Processor, Memory and Storage. Initialized to 0 */
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
        free_int_matrix(requirements,h_size);
    }

    return utilization;
}


/* load_weighted_sums: calculate the Weighted Sum of each solution
 * parameter: matrix that keep each objective function(power consumption,economical revenue,quality of service, resource wastage) value of individuals
 * parameter: array of weighted sums values
 * parameter: population matrix
 * parameter: the utilization matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * parameter: counter of the times the objective functions are calculated
 * returns: an array with the values of the objective function of each solution
 */
float* load_weighted_sums(float **objective_functions_values_aux, float *weighted_sums, int **population,
                          int ***utilization, int **H, float **V, int number_of_individuals, int h_size, int v_size,int* OF_calc_count)
{
    /* iterators */
    int iterator_individual,iterator_physical,iterator_virtual,physical_position;
    /* utility of a physical machine */
    float utilidad;
    /**/
    float power_consumption,economical_revenue, quality_of_service, wasted_resources_ratio;
    /**/
    float wasted_cpu_resources, wasted_ram_resources , wasted_net_resources;
    float wasted_cpu_resources_ratio, wasted_ram_resources_ratio, wasted_net_resources_ratio;
    float alpha = 1.0, beta = 1.0, gamma = 1.0;
    /**/
    int working_pms;


    /* value solution holds the weighted sum of each solution */
    /* iterate on individuals */
    for (iterator_individual = 0; iterator_individual < number_of_individuals; iterator_individual++) {

        /*counts the times that the FO are calculated*/
        *OF_calc_count+=1;

        /* (OF2) calculate energy consumption of each solution*/
        /* (OF4) calculate the wasted of resources of each solution */
        /* iterate on physical machines */
        working_pms=0,wasted_cpu_resources = 0.0 , wasted_ram_resources = 0.0 , wasted_net_resources = 0.0,power_consumption = 0.0,
        wasted_cpu_resources_ratio=0.0, wasted_ram_resources_ratio=0.0, wasted_net_resources_ratio=0.0;
        for (iterator_physical = 0; iterator_physical < h_size; iterator_physical++) {
            if (utilization[iterator_individual][iterator_physical][0] > 0 || utilization[iterator_individual][iterator_physical][1] > 0 || utilization[iterator_individual][iterator_physical][2] > 0) {

                /* calculates utility of a physical machine */
                utilidad = (float) utilization[iterator_individual][iterator_physical][0] / H[iterator_physical][0];
                /* calculates energy consumption of a physical machine */
                power_consumption +=
                        ((float) H[iterator_physical][3] - ((float) H[iterator_physical][3] * 0.6)) * utilidad +
                        (float) H[iterator_physical][3] * 0.6;

                /*calculate the wasted of resources (cpu,ram,net) */
                working_pms++;
                wasted_cpu_resources += 1 - (float)utilization[iterator_individual][iterator_physical][0] / H[iterator_physical][0];
                wasted_ram_resources += 1 - (float)utilization[iterator_individual][iterator_physical][1] / H[iterator_physical][1];
                wasted_net_resources += 1 - (float)utilization[iterator_individual][iterator_physical][2] / H[iterator_physical][2];
            }
        }

        /*calculate the ratio of wasted resources */
        if(working_pms>0) {
            wasted_cpu_resources_ratio = wasted_cpu_resources / working_pms;
            wasted_ram_resources_ratio = wasted_ram_resources / working_pms;
            wasted_net_resources_ratio = wasted_net_resources / working_pms;
        }
        wasted_resources_ratio = ( wasted_cpu_resources_ratio * alpha + wasted_ram_resources_ratio * beta + wasted_net_resources_ratio * gamma ) / 3;

        economical_revenue = 0.0;
        quality_of_service = 0.0;
        /* (OF1) calculate the economical revenue of each solution*/
        /* (OF3) calculate the QoS of each solution*/
        /* iterate on physical machines */
        for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
        {
            physical_position = population[iterator_individual][iterator_virtual];
            if (physical_position > 0)
            {
                /* calculate the economical revenue */
                economical_revenue += V[iterator_virtual][4];
                /* calculate  the QoS */
                quality_of_service += (float)pow(CONSTANT, V[iterator_virtual][3]) * V[iterator_virtual][3];
            }
        }

        /*compute the weighted sum based on the values of objective functions obtained and the weights configured*/
        weighted_sums[iterator_individual] = calculates_weighted_sum(power_consumption,economical_revenue,wasted_cpu_resources_ratio,quality_of_service);
        //printf("\t%.2f %.2f %.2f",power_consumption,economical_revenue,quality_of_service);

        objective_functions_values_aux[iterator_individual][0] = economical_revenue;
        objective_functions_values_aux[iterator_individual][1] = power_consumption;
        objective_functions_values_aux[iterator_individual][2] = quality_of_service;
        objective_functions_values_aux[iterator_individual][3] = wasted_resources_ratio;

    }
    return weighted_sums;
}

/**
 * calculates_weighted_sum: Calculates Weighted Sum of the Objetive Functions
 * parameter: power                  OF.2 Power Comsuption
 * parameter: total_revenue          OF.1 Economical Revenue
 * parameter: wasted_resources_ratio OF.4 Wasted Resources Ratio
 * parameter: total_qos              OF.3 Quality of Service
 *
 * return: the weighted sum
 */
float calculates_weighted_sum(float power, float total_revenue, float wasted_resources_ratio, float total_qos){

    float power_normalized = power * (float)SIGMA_POWER;
    float revenue_normalized = total_revenue * (float)SIGMA_REVENUE;
    float wasted_resources_normalized = wasted_resources_ratio * (float)SIGMA_RESOURCES;
    float qos_normalized = total_qos * (float)SIGMA_QOS;

    return ( power_normalized + revenue_normalized + wasted_resources_normalized + qos_normalized )/4;

}



/* get_best_solution_index: gets the index of the solution with the best value for the objective function
 * parameter: the array that contains the  weighted sums of individuals
 * parameter: the number of individuals
 * returns: the index of the individual that represents de the solution selected
 */
int get_best_solution_index(float* weighted_sums, int number_of_individuals){

    int iterator;
    int index_best_solution=0;
    float max_value = weighted_sums[0];
    for(iterator=1;iterator<number_of_individuals;iterator++){
        if(weighted_sums[iterator]>max_value){
            index_best_solution = iterator;
            max_value = weighted_sums[iterator];
        }
    }

    return index_best_solution;
}



/* report_solution: reports in files the utilization of the datacenter physical machines and the solution obtained
 * parameter: the solution array
 * parameter: the utilization matrix of the solution
 * parameter: the value of the objective function
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * parameter: time t
 * returns: nothing, it's void
 */
void report_solution(int *best_solution, int** utilization, float weighted_sum, int h_size, int v_size, int t){

    FILE *solutions_t;
    FILE *cpu_utilization_t;
    FILE *ram_utilization_t;
    FILE *net_utilization_t;
    int iterator;
    solutions_t = fopen("results/solutions","a+");
    cpu_utilization_t = fopen("results/cpu_utilization","a+");
    ram_utilization_t = fopen("results/ram_utilization","a+");
    net_utilization_t = fopen("results/net_utilization","a+");

    //fprintf(solutions_t,"Final placement obtained for t=%d\n",t);

    for(iterator=0;iterator<v_size;iterator++){

        fprintf(solutions_t, "%d\t", best_solution[iterator]);
    }

    //fprintf(solutions_t, "\n\nObjective Function Value for t=%d\n%.3f\n\n", t, weighted_sum);
    fprintf(solutions_t, "\n%.3f\n\n", weighted_sum);

    //fprintf(cpu_utilization_t,"CPU utilization for t=%d\n",t);
    for(iterator=0;iterator<h_size;iterator++){
        fprintf(cpu_utilization_t,"%d\t",utilization[iterator][0]);
    }
    fprintf(cpu_utilization_t,"\n\n");


    //fprintf(ram_utilization_t,"RAM utilization for t=%d\n",t);
    for(iterator=0;iterator<h_size;iterator++){
        fprintf(ram_utilization_t,"%d\t",utilization[iterator][1]);
    }
    fprintf(ram_utilization_t,"\n\n");


    //fprintf(net_utilization_t,"NET utilization for t=%d\n",t);
    for(iterator=0;iterator<h_size;iterator++){
        fprintf(net_utilization_t,"%d\t",utilization[iterator][2]);
    }
    fprintf(net_utilization_t,"\n\n");

    fclose(solutions_t);
    fclose(cpu_utilization_t);
    fclose(ram_utilization_t);
    fclose(net_utilization_t);
}

/* free_utilization_matrix: free memory allocated for the utilization three dimensional matrix
 * parameter: utilization matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * returns: nothing, it's void
 */
void free_utilization_matrix(int ***utilization,int individuals,int h_size){

    int iterator_individual,iterator_physical;
    for(iterator_individual=0;iterator_individual<individuals;iterator_individual++){
        for(iterator_physical=0;iterator_physical<h_size;iterator_physical++){
            free(utilization[iterator_individual][iterator_physical]);
        }
        free(utilization[iterator_individual]);
    }
    free(utilization);
}

/* free_float_matrix: free memory allocated for a float matrix
 * parameter: float matrix
 * parameter: number of rows
 * returns: nothing, it's void
 */
void free_float_matrix(float** matrix, int rows){

    int iterator;
    for(iterator=0;iterator<rows;iterator++){
        free(matrix[iterator]);
    }
    free(matrix);
}


/* free_int_matrix: free memory allocated for a int  matrix
 * parameter: int matrix
 * parameter: number of rows
 * returns: nothing, it's void
 */
void free_int_matrix(int** matrix, int rows){

    int iterator;
    for(iterator=0;iterator<rows;iterator++){
        free(matrix[iterator]);
    }
    free(matrix);
}


/* copy_int_float_matrix: copy the values of one matrix to another matrix with the same dimensions
 * parameter: matrix A
 * parameter  matrix B
 * parameter: number of rows
 * parameter: number of columns
 * returns: nothing, it's void
 */
void copy_int_matrix(int**matrix_A, int** matrix_B,int rows, int columns){

    int iterator_row,iterator_column;

    for (iterator_row = 0; iterator_row < rows; ++iterator_row) {
        for (iterator_column = 0; iterator_column < columns ; ++iterator_column) {
            matrix_A[iterator_row][iterator_column] = matrix_B[iterator_row][iterator_column];
        }
    }

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
