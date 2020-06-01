//
// Created by aherrerac on 5/5/20.
//

#ifndef CACHESIMULATOR_MESSAGE_H
#define CACHESIMULATOR_MESSAGE_H

typedef struct 
{
    int id;
    int request;
    int addr;
    int state;
} message;

typedef struct 
{
    int type;
    int source;
    int state;
    int destination;
    int address;
    int id; 
} nodeMessage;

typedef struct 
{
    int acction;
    int addr;
    int data;
} privateMessage;



#endif //CACHESIMULATOR_MESSAGE_H
