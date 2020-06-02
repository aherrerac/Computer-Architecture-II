#include <stdint.h>

typedef struct
{
    unsigned int id :1;
}bit ;

typedef struct 
{
    bit bit[4];
} bitArray;

typedef struct {
    unsigned int id : 1;
    unsigned int state: 3;
    unsigned int address : 5;
    int32_t data;
} L1block;

typedef struct {
    unsigned int state: 4;
    unsigned int pOwner : 1;
    unsigned int id : 2;
    unsigned int address : 5;
    unsigned int sharedExternal : 1;
    int32_t data; 
} L2block ;

typedef struct {
    unsigned int state : 2;
    unsigned int id : 5;
    int32_t data;
    bitArray procs; 
    int chips[2];
} mblock;