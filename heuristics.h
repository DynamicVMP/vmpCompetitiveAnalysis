/* 
 * heuristics.h: Virtual Machine Placement Problem - Heuristics Functions Header
 * Date: 21-02-2016
 * Author: Saúl Zalimben (szalimben93@gmail.com)
 */

/* include libraries */
#include "common.h"
 
/* definitions */
typedef struct PM_weight_pair_node{
    int h_index;
    float weight;
    struct PM_weight_pair_node* next;
} PM_weight_pair_node;



bool check_resources(float *request, float *utilization, int *H);
bool allocate_VM_to_PM(int **placement, float *request, int pm);

/* get the number of physical and virtual machines */
// int first_fit(float *S, float **utilization, int ****placement, int **H, int h_size);
int first_fit(float *S, float **utilization, int **placement, int **H, int h_size);

int best_fit(float *S, float **utilization, int **placement, int **H, int h_size);

int worst_fit(float *S, float **utilization, int **placement, int **H, int h_size);

int best_or_worst_fit(bool is_best, float *S, float **utilization, int **placement, int **H, int h_size);

float calculate_weight(int **placement, int *H, int h_index);

void insert_PM_to_ordered_list(bool is_best, PM_weight_pair_node** PM_ordered_list, float weight_PM, int h_index);

bool best_comparator(float weight_A, float weight_B);

bool worst_comparator(float weight_A, float weight_B);