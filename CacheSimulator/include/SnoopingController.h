//
// Created by aherrerac on 5/5/20.

#ifndef CACHESIMULATOR_SNOOPINGCONTROLLER_H
#define CACHESIMULATOR_SNOOPINGCONTROLLER_H

#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "message.h"
#include "instruction.h"
#include "cache.h"

void proController (message * message);
void busController (message * message);
privateMessage  prcacheController(instruction * newInstruction,L1 * L1);
privateMessage buscacheController(privateMessage * message,L1 * L1cache);
int findAddress(int address,L1 * L1cache);

#endif //CACHESIMULATOR_SNOOPINGCONTROLLER_H
