/* include libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "print_functions.h"

/* definitions */
#define S_HEADER "SCENARIOS"
#define TAM_BUFFER BUFSIZ


/* function headers definitions */
int get_s_size(char path_to_file[]);

float** load_S(int s_size, char path_to_file[]);
