#ifndef CACHESIMULATOR_BUS_H
#define CACHESIMULATOR_BUS_H

#include <stdint.h>
typedef struct
{
    unsigned int action : 3;
    unsigned int id : 1;
    unsigned int address : 4;
    int32_t data;
} bus;


typedef struct
{
    unsigned int chip : 1;
    unsigned int action : 3;
    unsigned int id : 1;
    unsigned int shared : 1;
    unsigned int address : 4;
    int32_t data;
} membus;

#endif //CACHESIMULATOR_BUS_H
