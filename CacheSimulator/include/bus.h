//
// Created by aherrerac on 30/5/20.
//

#ifndef CACHESIMULATOR_BUS_H
#define CACHESIMULATOR_BUS_H

#include <stdint.h>
typedef struct
{
    unsigned int state : 3;
    unsigned int address : 4;
    int32_t data;
} bus;

#endif //CACHESIMULATOR_BUS_H
