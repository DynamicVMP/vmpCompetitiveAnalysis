/*
 * common.c: Virtual Machine Placement Problem - Common Functions
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include common header */
#include "common.h"



/**
 * get_s_size: get the number of request
 * parameter: path to file
 * returns: number of request
 */
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
            /* if it is the correct block in the file, it is not the header and it is not a blank line or carriage return (ascii 13), we count */
            if (reading_scenario == 1 && strstr(input_line,S_HEADER) == NULL && strcmp(input_line, "\n") != 0
                && input_line[0] != 13) {
                s_size++;
            }
        }
    }
    /* close the file */
    fclose(scenario_file);
    /* return the value */
    return s_size;
}

/**
 * load_S: load the scenario
 * parameter: s_size: nummber of request
 * parameter: path to file
 * return: request matrix
 */
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
            if (reading_scenario == 1 && strstr(input_line,S_HEADER) == NULL && strcmp(input_line, "\n") != 0 && input_line[0] != 13) {
                S[iterator] = (float *) malloc (14 *sizeof (float));
                sscanf(input_line,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",&S[iterator][0],&S[iterator][1],&S[iterator][2],&S[iterator][3],&S[iterator][4],&S[iterator][5],&S[iterator][6],&S[iterator][7],&S[iterator][8],&S[iterator][9],&S[iterator][10],&S[iterator][11],&S[iterator][12],&S[iterator][13]);
                iterator++;
            }
        }
    }
    fclose(scenario_file);
    return S;
}

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
float ** load_v_per_t(float ** matrix_s, int s_size, int v_size, double *qos_a_priori_t, float *revenue_a_priori_t, int t){

    int iterator,time_vm_living;
    float **matrix_v = (float**)malloc(v_size*sizeof(float*));
    for(iterator=0;iterator<v_size;iterator++){
        matrix_v[iterator] = (float *) malloc(VM_FEATURES * sizeof(float));
    }

    int iterator_v= 0;
    iterator = 0;
    *revenue_a_priori_t=0.0;
    *qos_a_priori_t =0.0;
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

                time_vm_living = t - (int)matrix_v[iterator_v][5] +1;
                *qos_a_priori_t += pow(CONSTANT, matrix_v[iterator_v][3]) * matrix_v[iterator_v][3];
                *revenue_a_priori_t += matrix_v[iterator_v][4]*time_vm_living;
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
            requirements[iterator_physical] = (int *) malloc (RESOURCES *sizeof (int));
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
double* load_weighted_sums(double **objective_functions_values, double *weighted_sums, int **population,
                           int ***utilization,float ** wasted_resources_obj, int **H, float **V, int number_of_individuals, int h_size, int v_size, double qos_a_priori_t, float revenue_a_priori_t, int* OF_calc_count, int t)
{
    /* iterators */
    int iterator_individual;

    double max_delta_qos_t=0;
    double min_delta_qos_t=0;
    float max_delta_revenue_t=0;
    float min_delta_revenue_t=0;
    float max_power_consumption_t=0;
    float min_power_consumption_t=0;
    float max_wasted_resources_t =0;
    float min_wasted_resources_t =0;
    float delta_revenue_tmp, delta_revenue_normalized,power_normalized, wasted_resources_normalized;
    double delta_qos_tmp,delta_qos_normalized;


    objective_functions_values = load_objective_functions(objective_functions_values,population,utilization,wasted_resources_obj,H,V,number_of_individuals,h_size,v_size,OF_calc_count,t);

    max_delta_revenue_t  = revenue_a_priori_t - (float)objective_functions_values[0][0];
    max_power_consumption_t  = (float)objective_functions_values[0][1];
    max_delta_qos_t= qos_a_priori_t - objective_functions_values[0][2] ;
    max_wasted_resources_t = (float)objective_functions_values[0][3];

    /*Calculacion of values to normalize objetice funtions*/
    for(iterator_individual=1;iterator_individual<number_of_individuals;iterator_individual++){
        delta_revenue_tmp = revenue_a_priori_t - (float)objective_functions_values[iterator_individual][0];
        if(delta_revenue_tmp<min_delta_revenue_t){
            min_delta_revenue_t = delta_revenue_tmp;
        }
        if(delta_revenue_tmp>max_delta_revenue_t){
            max_delta_revenue_t = delta_revenue_tmp;
        }

        if(objective_functions_values[iterator_individual][1]<min_power_consumption_t){
            min_power_consumption_t = (float)objective_functions_values[iterator_individual][1];
        }
        if(objective_functions_values[iterator_individual][1]>max_power_consumption_t){
            max_power_consumption_t = (float)objective_functions_values[iterator_individual][1];
        }

        delta_qos_tmp = qos_a_priori_t - objective_functions_values[iterator_individual][2];
        if(delta_qos_tmp<min_delta_qos_t){
            min_delta_qos_t = delta_qos_tmp;
        }
        if(delta_qos_tmp>max_delta_qos_t){
            max_delta_qos_t = delta_qos_tmp;
        }

        if(objective_functions_values[iterator_individual][3] < min_wasted_resources_t){
            min_wasted_resources_t = (float)objective_functions_values[iterator_individual][3];
        }
        if(objective_functions_values[iterator_individual][3] > max_wasted_resources_t){
            max_wasted_resources_t = (float)objective_functions_values[iterator_individual][3];
        }
    }

    for(iterator_individual=0;iterator_individual<number_of_individuals;iterator_individual++){

        delta_revenue_tmp = revenue_a_priori_t - (float)objective_functions_values[iterator_individual][0];
        delta_qos_tmp = qos_a_priori_t - objective_functions_values[iterator_individual][2];

        //printf("\nrevenue :%f, power:%f, qos:%f, waste:%f",delta_revenue_tmp,objective_functions_values[iterator_individual][1],delta_qos_tmp,objective_functions_values[iterator_individual][3]);
        if(delta_revenue_tmp==min_delta_revenue_t){
            delta_revenue_normalized=0;
        }else {
            delta_revenue_normalized = (delta_revenue_tmp - min_delta_revenue_t) / (max_delta_revenue_t - min_delta_revenue_t);
        }

        if(objective_functions_values[iterator_individual][1]==min_power_consumption_t){
            power_normalized=0;
        }else{
            power_normalized = (float)(objective_functions_values[iterator_individual][1] - min_power_consumption_t)/(max_power_consumption_t-min_power_consumption_t);
        }

        if(delta_qos_tmp==min_delta_qos_t){
            delta_qos_normalized=0;
        }else{
            delta_qos_normalized = (float)(delta_qos_tmp - min_delta_qos_t)/(max_delta_qos_t-min_delta_qos_t);
        }

        if(objective_functions_values[iterator_individual][3] == min_wasted_resources_t){
            wasted_resources_normalized =0;
        }else{
            wasted_resources_normalized = (float)(objective_functions_values[iterator_individual][3] -
                                                  min_wasted_resources_t) / (
                                                  max_wasted_resources_t - min_wasted_resources_t);
        }

        //printf("\nrevenue normalized:%f, power normalized:%f, qos normalized:%f, waste normalized:%f\n",delta_revenue_normalized,power_normalized,delta_qos_normalized,wasted_resources_normalized);

        /*compute the weighted sum based on the values of objective functions obtained and the weights configured*/
        weighted_sums[iterator_individual] = calculates_weighted_sum(power_normalized, delta_revenue_normalized,
                                                                     wasted_resources_normalized, delta_qos_normalized);
    }

    //printf("\t%.2f %.2f %.2f",power_consumption,economical_revenue,quality_of_service,);

    return weighted_sums;
}


double** load_objective_functions(double **objective_functions_values, int **population, int ***utilization, float** wasted_resources_obj,int **H, float **V, int number_of_individuals, int h_size, int v_size, int* OF_calc_count, int t){


    /* iterators */
    int iterator_individual,iterator_physical,iterator_virtual,physical_position,time_vm_living;
    /* utility of a physical machine */
    float utilidad;
    /**/
    float power_consumption,economical_revenue, wasted_resources_ratio;
    double quality_of_service;
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

        /*save wasted resources*/
        wasted_resources_obj[iterator_individual][0] = wasted_cpu_resources_ratio;
        wasted_resources_obj[iterator_individual][1] = wasted_ram_resources_ratio;
        wasted_resources_obj[iterator_individual][2] = wasted_net_resources_ratio;


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
                /*time_vm_living = tend - tinit + 1*/
                time_vm_living  = t - (int)V[iterator_virtual][5]+1;
                /* calculate the economical revenue */
                economical_revenue += V[iterator_virtual][4]*time_vm_living;
                /* calculate  the QoS */
                quality_of_service += pow(CONSTANT, V[iterator_virtual][3]) * V[iterator_virtual][3];
            }else{
                /*the VM is considered*/
                if(V[iterator_virtual][0]>0) {
                    time_vm_living  = t - (int)V[iterator_virtual][5]+1;
                    /* calculate the economical revenue */
                    economical_revenue += 0.3 * V[iterator_virtual][4] * time_vm_living;
                }
            }
        }

        objective_functions_values[iterator_individual][0] = economical_revenue;
        objective_functions_values[iterator_individual][1] = power_consumption;
        objective_functions_values[iterator_individual][2] = quality_of_service;
        objective_functions_values[iterator_individual][3] = wasted_resources_ratio;

    }

    return objective_functions_values;

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
double calculates_weighted_sum(float power_normalized, float delta_revenue_normalized, float wasted_resources_normalized, double delta_qos_normalized){

    return power_normalized*SIGMA_POWER + delta_revenue_normalized*SIGMA_REVENUE + wasted_resources_normalized*SIGMA_RESOURCES + delta_qos_normalized*SIGMA_QOS ;

}



/* get_best_solution_index: gets the index of the solution with the best value for the objective function
 * parameter: the array that contains the  weighted sums of individuals
 * parameter: the number of individuals
 * returns: the index of the individual that represents de the solution selected
 */
int get_best_solution_index(double* weighted_sums, int number_of_individuals){

    int iterator;
    int index_best_solution=0;
    double min_value = weighted_sums[0];
    for(iterator=1;iterator<number_of_individuals;iterator++){
        if(weighted_sums[iterator]<min_value){
            index_best_solution = iterator;
            min_value = weighted_sums[iterator];
        }
    }

    return index_best_solution;
}

int get_worst_solution_index(double* weighted_sums, int number_of_individuals){

    int iterator;
    int index_worst_solution=0;
    double max_value = weighted_sums[0];
    for(iterator=1;iterator<number_of_individuals;iterator++){
        if(weighted_sums[iterator]>max_value){
            index_worst_solution = iterator;
            max_value = weighted_sums[iterator];
        }
    }

    return index_worst_solution;
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
void report_solution(int *best_solution,double* objective_functions_solution, int** utilization, double weighted_sum,float* wasted_resources_obj,float** V, int h_size, int v_size,char* file_postfix, int t){

    FILE *solutions_file;
    FILE *cpu_utilization_file;
    FILE *ram_utilization_file;
    FILE *net_utilization_file;
    FILE *vm_requests_derived_file;
    FILE *vm_requests_derived_count_file;
    FILE *weighted_sums_file;
    FILE *power_consumption_file;
    FILE *economical_revenue_file;
    FILE *qos_file;
    FILE *wasted_resources_file;
    FILE *working_pms_count_file;
    FILE *total_vm_requests_file;
    FILE *vm_requests_served_file;
    FILE *wasted_cpu_resources_file;
    FILE *wasted_ram_resources_file;
    FILE *wasted_net_resources_file;


    char file_name[300];
    int vms_derived_count=0, working_pms_count=0,vm_requests_served_count=0;


    int iterator,iterator2;
    bool flag_derived = false;
    sprintf(file_name,"results/solutions-%s",file_postfix);
    solutions_file = fopen(file_name, "a+");

    sprintf(file_name,"results/cpu_utilization-%s",file_postfix);
    cpu_utilization_file = fopen(file_name, "a+");

    sprintf(file_name,"results/ram_utilization-%s",file_postfix);
    ram_utilization_file = fopen(file_name, "a+");

    sprintf(file_name,"results/net_utilization-%s",file_postfix);
    net_utilization_file = fopen(file_name, "a+");

    sprintf(file_name,"results/derived_vms-%s",file_postfix);
    vm_requests_derived_file = fopen(file_name, "a+");

    sprintf(file_name,"results/working_pms-%s",file_postfix);
    working_pms_count_file = fopen(file_name, "a+");

    sprintf(file_name,"results/vm_requests-%s",file_postfix);
    total_vm_requests_file = fopen(file_name, "a+");

    sprintf(file_name,"results/vm_requests_served-%s",file_postfix);
    vm_requests_served_file = fopen(file_name, "a+");

    sprintf(file_name,"results/derived_vms_count-%s",file_postfix);
    vm_requests_derived_count_file = fopen(file_name, "a+");

    sprintf(file_name,"results/weighted_sums-%s",file_postfix);
    weighted_sums_file = fopen(file_name, "a+");

    sprintf(file_name,"results/power_consumption-%s",file_postfix);
    power_consumption_file = fopen(file_name, "a+");

    sprintf(file_name,"results/economical_revenue-%s",file_postfix);
    economical_revenue_file = fopen(file_name, "a+");

    sprintf(file_name,"results/quality_of_service-%s",file_postfix);
    qos_file = fopen(file_name, "a+");

    sprintf(file_name,"results/wasted_resources-%s",file_postfix);
    wasted_resources_file = fopen(file_name, "a+");

    sprintf(file_name,"results/wasted_cpu_resources-%s",file_postfix);
    wasted_cpu_resources_file = fopen(file_name, "a+");

    sprintf(file_name,"results/wasted_ram_resources-%s",file_postfix);
    wasted_ram_resources_file = fopen(file_name, "a+");

    sprintf(file_name,"results/wasted_net_resources-%s",file_postfix);
    wasted_net_resources_file = fopen(file_name, "a+");

    //fprintf(solutions_file,"Final placement obtained for t=%d\n",t);
    for(iterator=0;iterator<v_size;iterator++){
        fprintf(solutions_file, "%d\t", best_solution[iterator]);
    }
    fprintf(solutions_file,"\n");

    fprintf(weighted_sums_file, "%.3f\n", weighted_sum);

    for(iterator=0;iterator<v_size;iterator++){
       if(V[iterator][0]>0){
           if(best_solution[iterator]==0){
               vms_derived_count++;

               for(iterator2=0;iterator2<VM_FEATURES;iterator2++){
                   fprintf(vm_requests_derived_file, "%.3f\t", V[iterator][iterator2]);
               }
               flag_derived=true;
               fprintf(vm_requests_derived_file, "\n");
           }else{
               vm_requests_served_count++;
           }
       }
    }
    if(flag_derived){
        fprintf(vm_requests_derived_file, "\n");
    }

    fprintf(vm_requests_derived_count_file, "%d", vms_derived_count);
    fprintf(vm_requests_derived_count_file, "\n");

    fprintf(vm_requests_served_file,"%d",vm_requests_served_count);
    fprintf(vm_requests_served_file,"\n");

    fprintf(total_vm_requests_file, "%d", v_size);
    fprintf(total_vm_requests_file, "\n");

    fprintf(economical_revenue_file,"%f",objective_functions_solution[0]);
    fprintf(economical_revenue_file,"\n");

    fprintf(power_consumption_file,"%f",objective_functions_solution[1]);
    fprintf(power_consumption_file,"\n");

    fprintf(qos_file,"%f",objective_functions_solution[2]);
    fprintf(qos_file,"\n");

    fprintf(wasted_resources_file,"%f",objective_functions_solution[3]);
    fprintf(wasted_resources_file,"\n");

    fprintf(wasted_cpu_resources_file, "%f", wasted_resources_obj[0]);
    fprintf(wasted_cpu_resources_file, "\n");

    fprintf(wasted_ram_resources_file, "%f", wasted_resources_obj[1]);
    fprintf(wasted_ram_resources_file, "\n");

    fprintf(wasted_net_resources_file, "%f", wasted_resources_obj[2]);
    fprintf(wasted_net_resources_file, "\n");


    for(iterator=0;iterator<h_size;iterator++){
        if(utilization[iterator][0]>0 && utilization[iterator][1]>0 && utilization[iterator][2]>0){
            working_pms_count++;
        }
        fprintf(cpu_utilization_file, "%d\t", utilization[iterator][0]);
        fprintf(ram_utilization_file, "%d\t", utilization[iterator][1]);
        fprintf(net_utilization_file, "%d\t", utilization[iterator][2]);
    }
    fprintf(cpu_utilization_file, "\n");
    fprintf(ram_utilization_file, "\n");
    fprintf(net_utilization_file, "\n");

    fprintf(working_pms_count_file, "%d", working_pms_count);
    fprintf(working_pms_count_file, "\n");


    fclose(solutions_file);
    fclose(cpu_utilization_file);
    fclose(ram_utilization_file);
    fclose(net_utilization_file);
    fclose(vm_requests_derived_file);
    fclose(vm_requests_derived_count_file);
    fclose(power_consumption_file);
    fclose(wasted_resources_file);
    fclose(economical_revenue_file);
    fclose(qos_file);
    fclose(weighted_sums_file);
    fclose(working_pms_count_file);
    fclose(total_vm_requests_file);
    fclose(vm_requests_served_file);
    fclose(wasted_cpu_resources_file);
    fclose(wasted_ram_resources_file);
    fclose(wasted_net_resources_file);
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


/* free_double_matrix: free memory allocated for a double matrix
 * parameter: double matrix
 * parameter: number of rows
 * returns: nothing, it's void
 */
void free_double_matrix(double** matrix, int rows){

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

void copy_int_array(int* array_A, int* array_B,int array_size){

    int iterator;
    for(iterator=0;iterator<array_size;iterator++){
        array_A[iterator] = array_B[iterator];
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
            printf("%f\t",matrix[iterator_row][iterator_column]);
        }
        printf("\n");
    }
}


/* print_double_matrix: prints on screen a double matrix
 * parameter: matrix to print
 * parameter: number of individuals
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_double_matrix(double **matrix, int rows, int columns)
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
            printf("%f\t",matrix[iterator_row][iterator_column]);
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
        printf("[DEBUG] [%d]: %f\n",iterator_column,array[iterator_column]);
    }
}

/* print_double_array: prints on screen a double array
 * parameter: array to print
 * parameter: number of columns
 * returns: nothing, it's void
 */
void print_double_array(double *array, int columns)
{
    /* iterators */
    int iterator_column;
    /* iterate on columns */
    for (iterator_column = 0; iterator_column < columns; iterator_column++)
    {
        printf("[DEBUG] [%d]: %f\n",iterator_column,array[iterator_column]);
    }
}


void update_previous_placement(int *best_solution,int v_size,int** previous_placement,int* previous_v_size){

    if(*previous_v_size != v_size){
        *previous_placement = (int*)malloc(v_size*sizeof(int));
    }

    *previous_v_size = v_size;
    copy_int_array(*previous_placement,best_solution,*previous_v_size);
}


void report_migrations(int* best_solution, int v_size, int* previous_placement,int previous_v_size,float **V,char* file_postfix){


    FILE* vm_migrations_count_file;
    FILE* vm_memory_migrations_file;
    char file_name[300];
    int virtual_iterator, total_memory_migration, total_vm_migrations;


    sprintf(file_name,"results/vm_migrations_count-%s",file_postfix);
    vm_migrations_count_file = fopen(file_name,"a+");

    sprintf(file_name,"results/vm_memory_migrations-%s",file_postfix);
    vm_memory_migrations_file = fopen(file_name,"a+");

    total_memory_migration =0;
    total_vm_migrations = 0;
    for(virtual_iterator=0;virtual_iterator<previous_v_size;virtual_iterator++){

        /*if the vm is considered and now is in a different pm*/
        if(V[virtual_iterator][0]>0 && previous_placement[virtual_iterator]!=best_solution[virtual_iterator]){
            total_vm_migrations+=1;
            total_memory_migration+=(int)V[virtual_iterator][1];//ram of vm
        }

    }

    fprintf(vm_memory_migrations_file,"%d\n",total_memory_migration);
    fprintf(vm_migrations_count_file,"%d\n",total_vm_migrations);

    fclose(vm_memory_migrations_file);
    fclose(vm_migrations_count_file);

}
