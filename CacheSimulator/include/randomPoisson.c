#include "randomPoisson.h"


int getAddres(gsl_rng * r){
    int k = (int) gsl_ran_poisson (r,7.5);
    if(k > 16){
        return 15;
    }
    else{
        return k;
    }
}

void * intGenPoisson(instruction * inst,gsl_rng * opSeed,gsl_rng * dirSeed,int core,int chip){
    inst->core = core;
    inst->chip =chip;

    int k =(int) gsl_ran_poisson (opSeed,1.0);
    
    switch (k) {
        case 0:
            inst->op = 0;
            inst->address = getAddres(dirSeed);
            break;
        case 1:
            inst->op = 1;
            break;
        case 2:
            inst->op = 2;
            inst->address = getAddres(dirSeed);
            inst->data = rand();
            break;
        default:
            inst->op = 2;
            inst->address = getAddres(dirSeed);
            inst->data = rand();
    }
    return NULL;

}