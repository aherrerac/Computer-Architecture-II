#include <stdint.h>

typedef struct {
    unsigned int id : 1;
    unsigned int dBit : 1;
    unsigned int vBit : 1;
    unsigned int sBit : 1;
    unsigned int address : 4;
    int32_t data;
} L1block;

typedef struct {
    unsigned int dBit : 1;
    unsigned int vBit : 1;
    unsigned int sBit : 1;
    unsigned int pOwner : 1;
    unsigned int id : 2;
    unsigned int address : 4;
    int32_t data; 
} L2block ;

typedef struct {
    unsigned int dBit : 1;
    unsigned int vBit : 1;
    unsigned int sBit : 1;
    unsigned int pOwner : 1;
    unsigned int id : 4;
    int32_t data;
    //TODO: Bit array; 
    int chips[2];
} mblock;