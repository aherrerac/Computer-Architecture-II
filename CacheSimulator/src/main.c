#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/memory.h"
#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "../include/instruction.h"



int main(){
    const gsl_rng_type * T;
    gsl_rng * r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    int core = 0;
    int p = 0;
    for(int i =0; i < 100; i++){
        instruction * newInstr = intGenPoisson(r,core,p);
        core = 1;
        p = 1;
        printf("%d\n",newInstr->address);
    }

    gsl_rng_free (r);
    return  0;
}
