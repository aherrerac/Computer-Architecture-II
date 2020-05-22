//
// Created by aherrerac on 19/5/20.
//

#ifndef CACHESIMULATOR_RANDOMPOISSON_H
#define CACHESIMULATOR_RANDOMPOISSON_H
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "../include/instruction.h"

int getAddres(gsl_rng * r);
instruction * intGenPoisson(gsl_rng * r,int core,int chip);

#endif //CACHESIMULATOR_RANDOMPOISSON_H
