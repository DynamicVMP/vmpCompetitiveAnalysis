/* 
 * pareto.h: Virtual Machine Placement Problem - Pareto Functions Header
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include functions */
struct pareto_element * pareto_create(int v_size, int *individual, float *objectives_functions);
struct pareto_element * pareto_insert(struct pareto_element *pareto_head, int v_size, int *individual, float *objectives_functions);
void pareto_remove_duplicates(struct pareto_element *pareto_head, int v_size);
float get_min_cost(struct pareto_element *pareto_head, int objective);
float get_max_cost(struct pareto_element *pareto_head, int objective);
void print_pareto_set(struct pareto_element *pareto_head, int v_size);
void print_pareto_front(struct pareto_element *pareto_head);
int load_pareto_size(struct pareto_element *pareto_head);
void report_best_population(struct pareto_element *pareto_head, int **G, int **T, int **H, int **V, int v_size, int h_size, int l_size);
