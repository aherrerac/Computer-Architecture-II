#ifndef CACHESIMULATOR_INSTRUCTION_H
#define CACHESIMULATOR_INSTRUCTION_H
#include <stdio.h>

typedef struct {
    unsigned int core : 1;
    unsigned int chip : 1;
    unsigned int op : 2;
    unsigned int address :5;
    int32_t data;
} instruction;
#endif //CACHESIMULATOR_INSTRUCTION_H
