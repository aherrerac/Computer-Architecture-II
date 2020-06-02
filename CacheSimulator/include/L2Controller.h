//
// Created by aherrerac on 1/6/20.
//

#ifndef CACHESIMULATOR_L2CONTROLLER_H
#define CACHESIMULATOR_L2CONTROLLER_H
#include "cache.h"
#include "message.h"
#include "cache.h"

L2message  prcacheL2Controller(L2message * newInstruction,L2 * L2cache);
L2message buscacheL2Controller(L2message* message,L2 * L2cache);
int findL2Address(int address, L2 * L2cache);


#endif //CACHESIMULATOR_L2CONTROLLER_H
