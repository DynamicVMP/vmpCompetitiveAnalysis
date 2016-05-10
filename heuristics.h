/* 
 * heuristics.h: Virtual Machine Placement Problem - Heuristics Functions Header
 * Date: February, 2016
 * Author: Rodrigo Ferreira (rodrigofepy@gmail.com) 
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */

/* include libraries */
#include "common.h"

#define SIGMA_POWER 1.329674004
#define SIGMA_REVENUE 1.719854165
#define SIGMA_RESOURCES 1.802271979
#define SIGMA_QOS 2.141194374

/* definitions */
typedef struct PM_weight_pair_node{
    int h_index;
    float weight;
    struct PM_weight_pair_node* next;
} PM_weight_pair_node;

typedef struct VM_linked_list{
    int vm_index;
    int service;
    int datacenter;
    int tend;
    int pm;
    int cpu;
    int ram;
    int net;
    float ram_utilization;
    float cpu_utilization;
    float net_utilization;
    long revenue;
    int SLA;
    struct VM_linked_list* next;
} VM_linked_list;

/* function headers definitions */
bool check_resources(float *request, float **utilization, float **resources_requested, int **H, VM_linked_list** vm_list, int physical_machine, bool update);
void allocate_VM_to_PM(int **placement, float **utilization, float **resources_requested, float *request, int pm);

/* heuristics functions */
bool first_fit(float *S, float **utilization, float **resources_requested, int **placement, int **H, int h_size, VM_linked_list** VM_list_derived, VM_linked_list** vm_list, VM_linked_list** VM_list_serviced, VM_linked_list** VM_list_serviced_derived);
bool best_fit(float *S, float **utilization, float **resources_requested, int **placement, int **H, int h_size, VM_linked_list** VM_list_derived, VM_linked_list** vm_list, VM_linked_list** VM_list_serviced, VM_linked_list** VM_list_serviced_derived);
bool worst_fit(float *S, float **utilization, float **resources_requested, int **placement, int **H, int h_size, VM_linked_list** VM_list_derived, VM_linked_list** vm_list, VM_linked_list** VM_list_serviced, VM_linked_list** VM_list_serviced_derived);
bool best_or_worst_fit(bool is_best, float *S, float **utilization, float **resources_requested, int **placement, int **H, int h_size, VM_linked_list** VM_list_derived, VM_linked_list** vm_list, VM_linked_list** VM_list_serviced, VM_linked_list** VM_list_serviced_derived);

/* heuristics auxiliar functions */
float calculate_weight(float **utilization, int *H, int h_index);
void insert_PM_to_ordered_list(bool is_best, PM_weight_pair_node** PM_ordered_list, float weight_PM, int h_index);
bool best_comparator(float weight_A, float weight_B);
bool worst_comparator(float weight_A, float weight_B);
void prepare_input_for_decreasing_heuristics(float **S, int s_size);
void quicksort_decreasing_sort(float **S, int first_index, int last_index);
int quicksort_partition(float **S, int first_index, int last_index);
int compare_requests(float* request_A, float* request_B);

void insert_VM_to_tend_list(VM_linked_list** vm_list, float * request, int h_index);
bool time_comparator(int time_A, int time_B);
bool update_VM_resources(int **placement, float **utilization, float **resources_requested, float *request, VM_linked_list** vm_list, int **H);
void update_VM_list(VM_linked_list** vm_tend_list, float *request, int physical_machine);
long remove_VM_by_time(VM_linked_list** vm_list, VM_linked_list** VM_list_derived, int **placement, float **utilization, float **resources_requested, int currnt_time, int h_size);

/* Print functions definitions */
void print_VM_list(VM_linked_list* list_to_free);
void print_PM_list(PM_weight_pair_node* list_to_free);

/* Auxiliar functions for calculations */
float calculate_average_from_array(float * array, int size);

/* Free memory functions definitions */
void free_VM_list(VM_linked_list* list_to_free);
void free_list(PM_weight_pair_node* list_to_free);

/* objective functions */
float power_consumption (float **utilization, int **H, int h_size);
void economical_revenue (VM_linked_list** vm_list, VM_linked_list** VM_list_derived, long *revenue, long *qos);
float wasted_resources (float **utilization, float **resources_requested, int **H, int h_size);
float calculates_weighted_sum(float power, long total_revenue, float wasted_resources_ratio, long total_qos);