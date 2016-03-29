/* 
 * heuristics.h: Virtual Machine Placement Problem - Heuristics Functions Header
 * Date: February, 2016
 * Author: Rodrigo Ferreira (rodrigofepy@gmail.com) 
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */

/* include libraries */
#include "common.h"

#define SIGMA_POWER 1
#define SIGMA_REVENUE 1
#define SIGMA_RESOURCES 1
#define SIGMA_QOS 1

/* definitions */
typedef struct PM_weight_pair_node{
    int h_index;
    float weight;
    struct PM_weight_pair_node* next;
} PM_weight_pair_node;

typedef struct VM_tend{
    int vm_index;
    int service;
    int datacenter;
    int tend;
    int pm;
    float ram_utilization;
    float cpu_utilization;
    float net_utilization;
    float revenue;
    float SLA;
    struct VM_tend* next;
} VM_tend;

/* function headers definitions */
bool check_resources(float *request, float **utilization, float **resources_requested, int **H, VM_tend** vm_list, int physical_machine);
void allocate_VM_to_PM(int **placement, float **utilization, float **resources_requested, float *request, int pm);

/* heuristics functions */
int first_fit(float *S, float **utilization, float **resources_requested, int **placement, int **H, int h_size, int *request_rejected, VM_tend** vm_list);
int best_fit(float *S, float **utilization, float **resources_requested, int **placement, int **H, int h_size, int *request_rejected, VM_tend** vm_list);
int worst_fit(float *S, float **utilization, float **resources_requested, int **placement, int **H, int h_size, int *request_rejected, VM_tend** vm_list);
int best_or_worst_fit(bool is_best, float *S, float **utilization, float **resources_requested, int **placement, int **H, int h_size, int *request_rejected, VM_tend** vm_list);

/* heuristics auxiliar functions */
float calculate_weight(float **utilization, int *H, int h_index);
void insert_PM_to_ordered_list(bool is_best, PM_weight_pair_node** PM_ordered_list, float weight_PM, int h_index);
bool best_comparator(float weight_A, float weight_B);
bool worst_comparator(float weight_A, float weight_B);
void prepare_input_for_decreasing_heuristics(float **S, int s_size);
void quicksort_decreasing_sort(float **S, int first_index, int last_index);
int quicksort_partition(float **S, int first_index, int last_index);
int compare_requests(float* request_A, float* request_B);

void insert_VM_to_tend_list(VM_tend** vm_list, float * request, int h_index);
bool time_comparator(int time_A, int time_B);
bool update_VM_resources(int **placement, float **utilization, float **resources_requested, float *request, VM_tend** vm_list, int **H);

/* Print functions definitions */
void print_VM_list(VM_tend* list_to_free);
void print_PM_list(PM_weight_pair_node* list_to_free);

/* Free memory functions definitions */
void free_VM_list(VM_tend* list_to_free);
void free_list(PM_weight_pair_node* list_to_free);

/* objective functions */
float power_consumption (float **utilization, int **H, int h_size);
void economical_revenue (VM_tend** vm_list, float *revenue, float *qos);
float wasted_resources (float **utilization, float **resources_requested, int **H, int h_size);
float calculatesWeightedSum(float power, float total_revenue, float wasted_resources_ratio, float total_qos);