//
// Created by aherrerac on 5/5/20.

#ifndef CACHESIMULATOR_L1CONTROLLER_H
#define CACHESIMULATOR_L1CONTROLLER_H

#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "message.h"
#include "instruction.h"
#include "cache.h"

L1message  prcacheController(instruction * newInstruction,L1 * L1);
L1message buscacheController(L1message * message,L1 * L1cache);
int findAddress(int address,L1 * L1cache);

#endif //CACHESIMULATOR_L1CONTROLLER_H
