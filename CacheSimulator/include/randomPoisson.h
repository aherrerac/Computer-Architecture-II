//
// Created by aherrerac on 19/5/20.
//

#ifndef CACHESIMULATOR_RANDOMPOISSON_H
#define CACHESIMULATOR_RANDOMPOISSON_H
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "../include/instruction.h"


void * intGenPoisson(instruction * inst,gsl_rng * opSeed,gsl_rng * dirSeed,int core,int chip);

#endif //CACHESIMULATOR_RANDOMPOISSON_H
