//
// Created by aherrerac on 5/5/20.
//
//Todo : modificar la respuesta de error
#include "SnoopingController.h"

privateMessage prcacheController(instruction * newInstruction,L1 * L1cache){
    //Check if address is in cache
    int fhit = 0;
    int i;
    for (i = 0; i <= 1; i++)
    {
        if(L1cache->l1Blocks[i].state != 3){
            if(L1cache->l1Blocks[i].address == newInstruction->address && i == 0){
            //Todo: Poner la impresion formal 
            //printf("Se encontro en el primer lugar\n");
            fhit = 1;
            break;
        }
        else if(L1cache->l1Blocks[i].address == newInstruction->address && i == 1){
            //Todo: Poner la impresion formal 
            fhit = 1;
            //printf("Se encontro en el segundo lugar\n");
        }
        }   
    }
    privateMessage memResponse; 
    if (fhit == 1){
        //printf("Diagram States Address in memory\n");
        switch (L1cache->l1Blocks[i].state)
        {
        case 0: //Modified (Exclusive)
            //printf("Block State -> Modified\n");
            if(newInstruction->op == 0){
                //Poner el equivalente a minutos
                memResponse.acction = 0;
                memResponse.addr = newInstruction->address;
                memResponse.data = newInstruction->data;
                return memResponse;
            }
            else if(newInstruction->op == 2){
                memResponse.acction = 0;
                memResponse.addr = newInstruction->address;
                memResponse.data = newInstruction->data;
                L1cache->l1Blocks[i] = newInstruction->data;
                return memResponse;
            }
            break;
        case 1: //Shared
            //printf("Block State -> Shared\n");
            if(newInstruction->op == 0){
                memResponse.acction = 0;
                return memResponse;

            }
            else if(newInstruction->op == 2){
                memResponse.acction = 3;
                memResponse.addr = newInstruction->address;
                memResponse.chip = newInstruction->chip;
                memResponse.core = newInstruction->core;
                return memResponse;
            }
            break;
        case 2: //Invalid
            //printf("Block State -> Invalided\n");
            if(newInstruction->op == 0){
                memResponse.acction = 0;
                memResponse.addr = newInstruction->address;
                memResponse.chip = newInstruction->chip;
                memResponse.core = newInstruction->core;
                return memResponse;
            }
            else if(newInstruction->op == 2){
                memResponse.acction = 1;
                memResponse.addr = newInstruction->address;
                memResponse.chip = newInstruction->chip;
                memResponse.core = newInstruction->core;
                return memResponse;
            }
            break;
        default:
            //printf("Error: Non knowed State\n");
            break;
        }
    }
    else if (fhit == 0){
        int location = newInstruction->address%2;
        printf("Location %d \n",location);
        if(L1cache->l1Blocks[location].state == 0){
            memResponse.acction = 4;
            memResponse.addr = L1cache->l1Blocks[location].address;
            memResponse.chip = newInstruction->chip;
            memResponse.core = newInstruction->core;
            memResponse.data = L1cache->l1Blocks[location].data;
            return memResponse;
        }
        else{
            //printf("Read Miss / Write Miss\n");
            if(newInstruction->op == 0){
                memResponse.acction = 1;
                memResponse.addr = newInstruction->address;
                memResponse.chip = newInstruction->chip;
                memResponse.core = newInstruction->core;
                return memResponse;
            }
            else if(newInstruction->op == 2)
            {
                memResponse.acction = 2;
                memResponse.addr = newInstruction->address;
                memResponse.chip = newInstruction->chip;
                memResponse.core = newInstruction->core;
                return memResponse;
            }
        }       
    }
        //printf("Error: Cache L1 Fail (Processor %d Chip %d )\n" ,newInstruction->core,newInstruction->chip);
        memResponse.acction = 4;
        return memResponse;
}

privateMessage buscacheController(privateMessage * message,L1 * L1cache){
    privateMessage memResponse; 
    int address = message->addr;
    int i;
    for (i = 0; i <= 1; i++)
    {
        if(L1cache->l1Blocks[i].address == address && i == 0){
            break;
        }        
    }
    switch (L1cache->l1Blocks[i].state)
    {
    case 0:
        if(message->acction == 2){
            L1cache->l1Blocks[i].state = 2;
            memResponse.acction = 0;
            memResponse.addr = message->addr;
            memResponse.data = L1cache->l1Blocks[i].data;
            return memResponse;
        }
        else{
            L1cache->l1Blocks[i].state = 1;
            memResponse.acction = 0;
            memResponse.addr = message->addr;
            memResponse.data = L1cache->l1Blocks[i].data;
            return memResponse;
        }
        break;
    case 1:
        if(message->acction == 1){
            memResponse.acction = 0;
            return memResponse;
        }
        else if (message->acction == 2)
        {
            L1cache->l1Blocks[i].state = 2;
            memResponse.acction = 0;
            return memResponse;
        }
        else
        {
            L1cache->l1Blocks[i].state = 2;
            memResponse.acction = 0;
            return memResponse;
        }
        
        break;
    }
    memResponse.acction = 4;
    return memResponse;
}