#include "SnoopingController.h"

//Handles processor requests
privateMessage prcacheController(instruction * newInstruction,L1 * L1cache){
    //Check if address is in cache
    int fhit = 0;
    int i;
    //Memory response
    privateMessage memResponse; 
    for (i = 0; i <= 1; i++)
    {
        if(L1cache->l1Blocks[i].state != 3){
            if(L1cache->l1Blocks[i].address == newInstruction->address && i == 0){
            fhit = 1;
            break;
        }
        else if(L1cache->l1Blocks[i].address == newInstruction->address && i == 1){
            fhit = 1;
            }
        }   
    }
    //Block in cache
    if (fhit == 1){
        switch (L1cache->l1Blocks[i].state)
        {
        case 0: //Modified (Exclusive)
            if(newInstruction->op == 0){ //Read
                memResponse.acction = 5; //Read Hit
                memResponse.addr = newInstruction->address;
                memResponse.data = L1cache->l1Blocks[i].data;
                return memResponse;
            }
            else if(newInstruction->op == 2){ //Write
                memResponse.acction = 4;  //Write hit
                memResponse.addr = newInstruction->address;
                memResponse.data = newInstruction->data;
                L1cache->l1Blocks[i].data = newInstruction->data;
                return memResponse;
            }
            else{
                //Error 
                exit(0);
            } 
            break;
        case 1: //Shared
            if(newInstruction->op == 0){ //Read 
                memResponse.acction = 5; //Read Hit
                memResponse.addr = newInstruction->address;
                memResponse.data = L1cache->l1Blocks[i].data;
                return memResponse;

            }
            else if(newInstruction->op == 2){ //Write
                memResponse.acction = 2; //Invalidate
                memResponse.addr = newInstruction->address;
                return memResponse;
            }
            else{
                exit(0);
            }
            break;
        case 2: //Invalid
            if(newInstruction->op == 0){ //Read
                memResponse.acction = 0; //ReadMiss
                memResponse.addr = newInstruction->address;
                return memResponse;
            }
            else if(newInstruction->op == 2){
                memResponse.acction = 1; //WriteMiss
                memResponse.addr = newInstruction->address;
                return memResponse;
            }
            else{
                exit(0);
            }
            break;
        }
    }
    else if (fhit == 0){
        int location = newInstruction->address%2; // Address new location in cache
        if(L1cache->l1Blocks[location].state == 0){ //Write Back block Modified
            memResponse.acction = 4; //Write Back
            memResponse.addr = L1cache->l1Blocks[location].address;
            memResponse.data = L1cache->l1Blocks[location].data;
            return memResponse;
        }
        else{
            if(newInstruction->op == 0){ //Read
                memResponse.acction = 0; //Read Miss
                memResponse.addr = newInstruction->address;
                return memResponse;
            }
            else if(newInstruction->op == 2){ //Write
                memResponse.acction = 1; //Write Miss
                memResponse.addr = newInstruction->address;
                return memResponse;
            }
        }       
    }
    exit(0);
}
//Handles bus responses 
privateMessage buscacheController(privateMessage * message,L1 * L1cache){
    privateMessage memResponse;
    int location = message->addr%2;
    switch (message->acction)
    {
    case 0:
        
        //Block already on cache
        if (L1cache->l1Blocks[location].address == message->addr){
            L1cache->l1Blocks[location].state = 1;
            L1cache->l1Blocks[location].data = message->data;
            memResponse.acction = 6; //Updated
            memResponse.addr = L1cache->l1Blocks[location].address;
            memResponse.data = L1cache->l1Blocks[location].data;
            return memResponse;
        }
        //New block
        else{
            L1cache->l1Blocks[location].address = message->addr;
            L1cache->l1Blocks[location].data = message->data;
            L1cache->l1Blocks[location].state = 1;
            memResponse.acction = 7; //Created
            memResponse.addr = L1cache->l1Blocks[location].address;
            memResponse.data = L1cache->l1Blocks[location].data;
            return memResponse;
        }
        break;
    case 1:
        //Block already on cache
        if (L1cache->l1Blocks[location].address == message->addr){
            L1cache->l1Blocks[location].state = 0;
            L1cache->l1Blocks[location].data = message->data;
            memResponse.acction = 6; //Updated
            memResponse.addr = L1cache->l1Blocks[location].address;
            memResponse.data = L1cache->l1Blocks[location].data;
            return memResponse;
        }
        //New block
        else{
            L1cache->l1Blocks[location].address = message->addr;
            L1cache->l1Blocks[location].data = message->data;
            L1cache->l1Blocks[location].state = 0;
            memResponse.acction = 7; //Created
            memResponse.addr = L1cache->l1Blocks[location].address;
            memResponse.data = L1cache->l1Blocks[location].data;
            return memResponse;
        }
        break;
    case 2:
        //Change block state to Invalid
        L1cache->l1Blocks[location].state = 2;
        memResponse.acction = 7; //Invalidated
        memResponse.addr = L1cache->l1Blocks[location].address;
        memResponse.data = L1cache->l1Blocks[location].data;
        return memResponse;
        break;
    case 3:
        L1cache->l1Blocks[location].state = 1;
        memResponse.acction = 9; //Shared
        memResponse.addr = L1cache->l1Blocks[location].address;
        memResponse.data = L1cache->l1Blocks[location].data;
        return memResponse;
        break;
    
    }
    exit(0);
}

int findAddress(int address,L1 * L1cache){
    int i;
    int fhit = 0;
    for (i = 0; i <= 1; i++)
    {
        if(L1cache->l1Blocks[i].state != 3){
            if(L1cache->l1Blocks[i].address == address && i == 0){
            fhit = 1;
            break;
            }
            else if(L1cache->l1Blocks[i].address == address && i == 1){
                fhit = 1;
            }
        }   
    }
    return fhit;
}