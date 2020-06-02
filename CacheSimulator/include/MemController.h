#ifndef CACHESIMULATOR_MEMCONTROLLER_H
#define CACHESIMULATOR_MEMCONTROLLER_H
#include "memory.h"
#include "message.h"

Mmessage prcacheL2Controller(Mmessage * message,memory * memory);
int getPosition(int id, int chip);

#endif //CACHESIMULATOR_MEMCONTROLLER_H
