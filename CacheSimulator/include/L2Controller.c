//
// Created by aherrerac on 1/6/20.
//

#include "L2Controller.h"

L2message  prcacheController(L2message * message,L2 * L2cache){
    L2message memResponse; 
    int location = message->addr%4;
    switch (message->acction)
    {
    case 0: //Read Miss
        if (L2cache->l2Blocks[location].address == message->addr)
        {
            if(L2cache->l2Blocks[location].state == 2){
                memResponse.acction = message->acction;
                memResponse.addr = L2cache->l2Blocks[location].address;
                memResponse.data = L2cache->l2Blocks[location].data;
                memResponse.id = message->id;
                return memResponse;
            }
            else
            {
                memResponse.acction = message->acction;
                memResponse.addr = message->addr;
                memResponse.id = L2cache->l2Blocks[location].pOwner;
                return memResponse;
            }
        }
        else{
            memResponse.acction = 4;
            memResponse.addr = message->addr;
            memResponse.id = message->id;
        }
        
        break;
    case 1: //WriteMiss
        if (L2cache->l2Blocks[location].address == message->addr && L2cache->l2Blocks[location].sharedExternal ==0)
        {
            memResponse.acction = 0;
            memResponse.id = L2cache->l2Blocks[location].pOwner;
            memResponse.addr = message->addr;
            return memResponse;
        }
        else
        {
            memResponse.acction = 5;
            memResponse.id = message->id;
            memResponse.addr = message->addr;
            return memResponse;
        }
        break;
    case 2:
        memResponse.acction = 6;
        memResponse.id = message->id;
        memResponse.addr = message->addr;
        return memResponse;
        break;
    case 3:
        memResponse.acction = 7;
        memResponse.id = message->id;
        memResponse.addr = message->addr;
        return memResponse;
        break;
    }

}
L2message buscacheController(L2message* message,L2 * L2cache){

}
int findAddress(int address, L2 * L2cache){
    int i;
    int fhit = 0;
    for (i = 0; i <= 3; i++)
    {
        if(L2cache->l2Blocks[i].state != 3){
            if(L2cache->l2Blocks[i].address == address && i == 0){
            fhit = 1;
            break;
            }
            else if(L2cache->l2Blocks[i].address == address && i == 1){
                fhit = 1;
            }
        }   
    }
    return fhit;
}
