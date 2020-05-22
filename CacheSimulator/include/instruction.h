#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


#ifndef CACHESIMULATOR_INSTRUCTION_H
#define CACHESIMULATOR_INSTRUCTION_H

typedef struct {
    unsigned int core : 1;
    unsigned int chip : 1;
    unsigned int op : 2;
    unsigned int address :4;
    int32_t data;
} instruction;

//instruction * intrGenerator(unsigned int core, unsigned int chip);

#endif //CACHESIMULATOR_INSTRUCTION_H
