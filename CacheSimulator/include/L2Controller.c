//
// Created by aherrerac on 1/6/20.
//

#include "L2Controller.h"

L2message  prcacheL2Controller(L2message * message,L2 * L2cache){
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
L2message buscacheL2Controller(L2message* message,L2 * L2cache){
    L2message memResponse; 
    int location = message->addr%4;
    switch (message->acction)
    {
    case 0:
        memResponse.acction = 0;
        memResponse.addr = message->addr;
        memResponse.id = L2cache->l2Blocks[location].pOwner;
        return memResponse;
        break;
    case 1:
        memResponse.acction = 2;
        memResponse.addr = message->addr;
        L2cache->l2Blocks[location].state = 2;
        if (L2cache->l2Blocks[location].state == 1)
        {
            memResponse.shared = 1;
            return memResponse;
        }
        else
        {
            memResponse.id = L2cache->l2Blocks[location].pOwner;
            memResponse.shared = 0;
            return memResponse;
        }
        break;
     case 2:
        memResponse.acction = 2;
        memResponse.addr = message->addr;
        L2cache->l2Blocks[location].state = 2;
        if (L2cache->l2Blocks[location].state == 1)
        {
            memResponse.shared = 1;
            return memResponse;
        }
        else
        {
            memResponse.id = L2cache->l2Blocks[location].pOwner;
            memResponse.shared = 0;
            return memResponse;
        }
        break;
    case 4:
        L2cache->l2Blocks[location].state = 1;
        L2cache->l2Blocks[location].address = message->addr;
        L2cache->l2Blocks[location].data = message->data;
        L2cache->l2Blocks[location].pOwner = message->id;
        L2cache->l2Blocks[location].sharedExternal = message->sharedExternal;
        memResponse.acction = 0;
        memResponse.addr = message->addr;
        memResponse.data = message->data;
        memResponse.id = message->id;
        return memResponse;
        break;
    case 5:
        L2cache->l2Blocks[location].state = 2;
        L2cache->l2Blocks[location].address = message->addr;
        L2cache->l2Blocks[location].data = message->data;
        L2cache->l2Blocks[location].pOwner = message->id;
        L2cache->l2Blocks[location].sharedExternal = message->sharedExternal;
        memResponse.acction = 1;
        memResponse.addr = message->addr;
        memResponse.data = message->data;
        memResponse.id = message->id;
        return memResponse;
        break;
    case 6:
        if (L2cache->l2Blocks[location].state = 1)
        {
            L2cache->l2Blocks[location].state = 2;
            memResponse.acction = 1;
            memResponse.addr = message->addr;
            memResponse.shared = 1;
            return memResponse;

        }
        else
        {
            L2cache->l2Blocks[location].state = 2;
            memResponse.acction = 1;
            memResponse.addr = message->addr;
            memResponse.shared = 0;
            return memResponse;
        }
        break;
    case 7:
        memResponse.acction = 3;
        memResponse.id = message->id;
        memResponse.addr = message->addr;
        return memResponse;
        break;
    }
}
int findL2Address(int address, L2 * L2cache){
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
