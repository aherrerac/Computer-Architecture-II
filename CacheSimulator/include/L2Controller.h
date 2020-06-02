//
// Created by aherrerac on 1/6/20.
//

#ifndef CACHESIMULATOR_L2CONTROLLER_H
#define CACHESIMULATOR_L2CONTROLLER_H
#include "cache.h"
#include "message.h"
#include "cache.h"

L2message  prcacheController(L2message * newInstruction,L2 * L2cache);
L2message buscacheController(L2message* message,L2 * L2cache);
int findAddress(int address, L1 * L1cache);


#endif //CACHESIMULATOR_L2CONTROLLER_H
