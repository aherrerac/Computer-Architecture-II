//
// Created by aherrerac on 5/5/20.
//

#ifndef CACHESIMULATOR_DIRECTORYCONTROLLER_H
#define CACHESIMULATOR_DIRECTORYCONTROLLER_H
#include <stdio.h>
#include "message.h"

void localDirController(struct nodeMessage * message);
void homeDirController(struct nodeMessage * message);
void remoteDirController(struct nodeMessage * message);
void directoryController(struct nodeMessage * message);

#endif //CACHESIMULATOR_DIRECTORYCONTROLLER_H
