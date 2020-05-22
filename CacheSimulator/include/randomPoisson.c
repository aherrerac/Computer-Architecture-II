#include "randomPoisson.h"


int getAddres(gsl_rng * r){
    int k = (int) gsl_ran_poisson (r,7.5);
    if(k > 15){
        return 15;
    }
    else
        return k;
}

instruction * intGenPoisson(gsl_rng * r,int core,int chip){
    instruction * inst = (instruction *) malloc(sizeof(instruction));
    inst->core = core;
    inst->chip =chip;

    int k = gsl_ran_poisson (r,1.0);
    switch (k) {
        case 0:
            inst->op = 0;
            inst->address = getAddres(r);
            break;
        case 1:
            inst->op = 1;
            break;
        case 2:
            inst->op = 2;
            inst->address = getAddres(r);
            inst->data = gsl_ran_poisson (r,55.0);
            break;
        default:
            inst->op = 2;
            inst->address = getAddres(r);
            inst->data = gsl_ran_poisson (r,55.0);
    }
    return inst;

}