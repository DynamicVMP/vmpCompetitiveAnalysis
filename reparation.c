/*
 * reparation.c: Virtual Machine Placement Problem - Reparation Stage
 * Date: 17-11-2014
 * Author: Fabio Lopez Pires (flopezpires@gmail.com)
 * Corresponding Conference Paper: A Many-Objective Optimization Framework for Virtualized Datacenters
 */

/* include reparation stage header */
#include "reparation.h"

/* reparation: repairs the population
 * parameter: population matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * parameter: the max level of SLA
 * returns: repaired population matrix
 */
int** reparation(int ** population, int *** utilization, int ** H, float ** V, int number_of_individuals, int h_size, int v_size, int max_SLA)
{
    /* repairs population from not feasible individuals */
    repair_population(population, utilization, H, V, number_of_individuals, h_size, v_size, max_SLA);
    return population;
}

/* repair_population: repairs population from not feasible individuals
 * parameter: population matrix
 * parameter: utilization of the physical machines matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * parameter: the max level of SLA
 * returns: nothing, it's void()
 */
void repair_population(int ** population, int *** utilization, int ** H, float ** V, int number_of_individuals, int h_size, int v_size, int max_SLA)
{
    /* iterators */
    int iterator_individual = 0;
    int feasibility = 1;
    /* iterate on individuals */
    for (iterator_individual = 0; iterator_individual < number_of_individuals ; iterator_individual++)
    {
        feasibility = check_feasibility(population,utilization,iterator_individual,H,V,h_size,v_size,max_SLA);

        /* if the individual is not feasible */
        if (feasibility == 0)
        {
            repair_individual(population, utilization, H, V, number_of_individuals, h_size, v_size, max_SLA,iterator_individual);
        }
    }
}

/* repair_individual: repairs not feasible individuals
 * parameter: population matrix
 * parameter: utilization of the physical machines matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * parameter: the max level of SLA
 * parameter: identifier of the not feasible individual to repair
 * returns: nothing, it's void()
 */
void repair_individual(int ** population, int *** utilization, int ** H, float ** V, int number_of_individuals, int h_size, int v_size, int max_SLA, int individual)
{
    int iterator_virtual = 0;
    int iterator_virtual_2 = 0;
    int iterator_physical = 0;
    int aux_cpu_sum = 0;
    int aux_mem_sum = 0;
    int aux_net_sum = 0;

    /* id of a candidate physical machine for migration */
    int candidate = 0;
    /* a migration flag for overloaded physical machines indicating that a virtual machine was or not migrated yet */
    int migration = 0;
    /* iterate on each virtual machine to search for overloaded physical machines */
    for (iterator_virtual = 0; iterator_virtual < v_size; iterator_virtual++)
    {
        /* if the virtual machine was placed */
        if (population[individual][iterator_virtual] != 0)
        {
            migration = 0;
            /* verify is the physical machine assigned is overloaded in any physical resource */
            if (is_overloaded(H, utilization, individual, (population[individual][iterator_virtual]-1)))
            {
                /* we search for a correct candidate for VM "migration" (it is not really a migration, only a physical machine change) */
                candidate = rand() % h_size;
                for (iterator_physical=0; iterator_physical < h_size; iterator_physical++)
                {
                    /* if the candidate can assume the resource requested */
                    if (utilization[individual][candidate][0] + V[iterator_virtual][0] <= H[candidate][0] &&
                        utilization[individual][candidate][1] + V[iterator_virtual][1] <= H[candidate][1] &&
                        utilization[individual][candidate][2] + V[iterator_virtual][2] <= H[candidate][2])
                    {
                        /* delete requirements from physical machine migration source */
                        utilization[individual][population[individual][iterator_virtual]-1][0] -= V[iterator_virtual][0];
                        utilization[individual][population[individual][iterator_virtual]-1][1] -= V[iterator_virtual][1];
                        utilization[individual][population[individual][iterator_virtual]-1][2] -= V[iterator_virtual][2];

                        /* add requirements from physical machine migration destination */
                        utilization[individual][candidate][0] += V[iterator_virtual][0];
                        utilization[individual][candidate][1] += V[iterator_virtual][1];
                        utilization[individual][candidate][2] += V[iterator_virtual][2];
                        /* refresh the population */
                        population[individual][iterator_virtual] = candidate + 1;

                        /* virtual machine correctly "migrated" */
                        migration = 1;
                        break;
                    }

                    if (candidate < (h_size - 1))
                    {
                        candidate++;
                    }
                    else
                    {
                        candidate = 0;
                    }
                }

                if (!migration)
                {
                    if (V[iterator_virtual][3]!=max_SLA)
                    {
                        /* delete requirements from physical machine migration source */
                        utilization[individual][population[individual][iterator_virtual]-1][0] -= V[iterator_virtual][0];
                        utilization[individual][population[individual][iterator_virtual]-1][1] -= V[iterator_virtual][1];
                        utilization[individual][population[individual][iterator_virtual]-1][2] -= V[iterator_virtual][2];

                        /* refresh the population */
                        population[individual][iterator_virtual] = 0;
                        /* virtual machine correctly "deleted" */
                        migration = 1;
                        break;
                    }
                }
                if(!migration)
                {
                    break;
                }
            }
        }
    }

    /* after everything else is ok, repair SLA placement priority */
    for (iterator_virtual = 0; iterator_virtual < v_size; iterator_virtual++)
    {
        /* if the VM has SLA(maxSLA) and is not marked off and was not placed */
        if  ((V[iterator_virtual][3] == max_SLA) && V[iterator_virtual][0]>0 && (population[individual][iterator_virtual] == 0))
        {
            /* iterate through all physical machines, starting at a random offset */
            candidate = (rand() % h_size) + 1; //1 >= candidate <= h_size
            for (iterator_physical=0; iterator_physical < h_size; iterator_physical++) {

                /* iterate again on all virtual machines */
                aux_cpu_sum, aux_net_sum, aux_mem_sum = 0;
                for (iterator_virtual_2 = 0; iterator_virtual_2 < v_size; iterator_virtual_2++)
                {
                    /* if the VM is placed on the candidate physical machine and has SLA!=maxSLA, add its resources */
                    if ((population[individual][iterator_virtual_2] == candidate) && (V[iterator_virtual_2][3] != max_SLA)){
                        aux_cpu_sum += V[iterator_virtual_2][0];
                        aux_mem_sum += V[iterator_virtual_2][1];
                        aux_net_sum += V[iterator_virtual_2][2];
                    }
                }

                /* if enough VMs with SLA!=maxSLA can be shutdown on PM(candidate) to accomodate the SLA(maxSLA) VM */
                if ((aux_cpu_sum >= V[iterator_virtual][0]) && (aux_mem_sum >= V[iterator_virtual][1]) && (aux_net_sum >= V[iterator_virtual][2])) {
                    /* iterate on virtual machines to put SLA(maxSLA) VM on PM(candidate) */
                    aux_cpu_sum, aux_net_sum, aux_mem_sum = 0;
                    for (iterator_virtual_2 = 0; iterator_virtual_2 < v_size; iterator_virtual_2++)
                    {
                        /* if the VM SLA != maxSLA and the VM was placed, add its resources */
                        if ((population[individual][iterator_virtual_2] == candidate) && (V[iterator_virtual_2][3] != max_SLA)){
                            aux_cpu_sum += V[iterator_virtual_2][0];
                            aux_mem_sum += V[iterator_virtual_2][1];
                            aux_net_sum += V[iterator_virtual_2][2];

                            /* delete requirements from candidate physical machine */
                            utilization[individual][candidate - 1][0] -= V[iterator_virtual_2][0];
                            utilization[individual][candidate - 1][1] -= V[iterator_virtual_2][1];
                            utilization[individual][candidate - 1][2] -= V[iterator_virtual_2][2];

                            /* shutdown the VM with SLA!=maxSLA */
                            population[individual][iterator_virtual_2] = 0;

                            /* if enough VMs were shut down, leave */
                            if ((aux_cpu_sum >= V[iterator_virtual][0]) && (aux_mem_sum >= V[iterator_virtual][1]) && (aux_net_sum >= V[iterator_virtual][2])) {
                                break;
                            }
                        }
                    }
                    /* add requirements of VM with SLA(maxSLA) */
                    utilization[individual][candidate - 1][0] += V[iterator_virtual][0];
                    utilization[individual][candidate - 1][1] += V[iterator_virtual][1];
                    utilization[individual][candidate - 1][2] += V[iterator_virtual][2];

                    /* place VM with SLA(maxSLA) to PM(candidate) */
                    population[individual][iterator_virtual] = candidate;

                    /* VM with SLA(maxSLA) successfully placed, continue to the next VM */
                    break;
                }

                if (candidate < h_size)
                {
                    candidate++;
                }
                else
                {
                    candidate = 1;
                }
            }
        }
    }

}

/* is_overloaded: verifies if a physical machine is overloaded
 * parameter: physical machine resources matrix
 * parameter: utilization of the physical machines matrix
 * parameter: identifier of the individual
 * parameter: identifier of the physical machine
 * returns: 1 if yes, 0 if no
 */
int is_overloaded(int ** H, int *** utilization, int individual, int physical)
{
    /* If the use of the VM exceeds the capacity of the physical machine return 1, otherwise return 0 */
    if ((utilization[individual][physical][0] > H[physical][0])
        ||  (utilization[individual][physical][1] > H[physical][1])
        ||  (utilization[individual][physical][2] > H[physical][2]))
    {
        return 1;
    }
    return 0;
}

/* check_feasibility: verifies if a individual represents a feasible solution
 * parameter: population matrix
 * parameter: utilization of the physical machines matrix
 * parameter: identifier of the individual
 * parameter: physical machine resources matrix
 * parameter: virtual machine requirements matrix
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * parameter: the max level of SLA
 * returns: 1 if yes, 0 if no
 */
int check_feasibility(int** population,int *** utilization,int iterator_individual,int ** H, float ** V,int h_size,int v_size,int max_SLA){


    int feasibility = 1;
    int iterator_virtual;
    int iterator_physical;
    int count_vm_sla_0_on=0, count_vm_sla_1_off =0;
    /* for now is not considered the SLA
    /* constraint 2: Service Level Agreement (SLA) provision. Virtual machines with SLA = max_SLA have to be placed mandatorily */
    for (iterator_virtual = 0; iterator_virtual < v_size; iterator_virtual++)
    {
        if (V[iterator_virtual][3] == max_SLA && population[iterator_individual][iterator_virtual] == 0)
        {
            feasibility = 0;
            break;
        }
        /* A vm marked as off (when CPU = 0) must not be placed */
        if ((V[iterator_virtual][0] == 0) && (population[iterator_individual][iterator_virtual] > 0)) {
            feasibility = 0;
            break;
        }

        /* Count VMs with SLA = max_SLA not market off and not placed */
        if ((V[iterator_virtual][3] == max_SLA) && (V[iterator_virtual][0] > 0) && (population[iterator_individual][iterator_virtual] == 0)) {
            count_vm_sla_1_off++;
        }

        /* Count placed VMs with SLA != max_SLA */
        if ((V[iterator_virtual][3] != max_SLA) && (population[iterator_individual][iterator_virtual] > 0) ) {
            count_vm_sla_0_on++;
        }

    }

    /* If there are placed VMs with not max_SLA  while VMs with SLA max_SLA were not placed, repair */
    if ((count_vm_sla_1_off > 0) && (count_vm_sla_0_on > 0)) {
        feasibility = 0;
    }

    /* constraints 3-5: Resource capacity of physical machines. Iterate on physical machines */
    for (iterator_physical = 0; iterator_physical < h_size ; iterator_physical++)
    {
        /* if any physical machine is overloaded on any resource, the individual is not feasible */
        if (is_overloaded(H, utilization, iterator_individual, iterator_physical))
        {
            feasibility = 0;
            break;
        }
    }

    return feasibility;

}
