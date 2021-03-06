#ifndef CACHESIMULATOR_MESSAGE_H
#define CACHESIMULATOR_MESSAGE_H

typedef struct 
{
    int id;
    int acction;
    int addr;
    int data;
    int sharedExternal;
    int shared;
} L2message;


typedef struct 
{
    int id;
    int acction;
    int addr;
    int data;
    int sharedExternal;
    int shared;
    int chip;
} Mmessage;

typedef struct 
{
    int acction;
    int addr;
    int data;
} L1message;



#endif //CACHESIMULATOR_MESSAGE_H
