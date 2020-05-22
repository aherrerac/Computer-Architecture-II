//
// Created by aherrerac on 5/5/20.
//

#ifndef CACHESIMULATOR_MESSAGE_H
#define CACHESIMULATOR_MESSAGE_H

struct message
{
    int id;
    int request;
    int addr;
    int state;
};

struct nodeMessage
{
    int type;
    int source;
    int state;
    int destination;
    int address;
    int id; 
};



#endif //CACHESIMULATOR_MESSAGE_H
