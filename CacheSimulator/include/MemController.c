#include "MemController.h"

int getPosition(int id, int chip){
    if(chip == 0){
        return id;
    }
    else
    {
        if (id == 0)
        {
            return 2;
        }
        else
        {
            return 3;
        }
    }
}
/*
Mmessage prcacheL2Controller(Mmessage * message,memory * memory){
    Mmessage systemRequest;
    if(memory->blocks[message->addr].state == 0){
        systemRequest.acction = message->acction;
        systemRequest.addr = message->addr;
        systemRequest.id = message->id;
        return systemRequest;
    }
    else{
        switch (message->acction)
        {
        case 4:
            if(memory->blocks[message->addr].state == 0){
                systemRequest.acction = message->acction;
                systemRequest.addr = message->addr;
                memory->blocks[message->addr].state = 1;
                memory->blocks[message->addr].procs.bit[getProcessorNum(message->id,message->chip)].id = 1;
            }
            else if(memory->blocks[message->addr].state == 2){
                systemRequest.acction = message->acction;
                systemRequest.addr = message->addr;
                memory->blocks[message->addr].state = 1;
                memory->blocks[message->addr].procs.bit[getProcessorNum(message->id,message->chip)].id = 1;
                return systemRequest; 
            }
            else {
                systemRequest.acction = message->acction;
                systemRequest.addr = message->addr;
                 memory->blocks[message->addr].procs.bit[getProcessorNum(message->id,message->chip)].id = 1;
                return systemRequest; 
            }
            break;
        case 5:
                if(memory->blocks[message->addr].state == 0){
                systemRequest.acction = message->acction;
                systemRequest.addr = message->addr;
                memory->blocks[message->addr].state = 1;
                memory->blocks[message->addr].procs.bit[getProcessorNum(message->id,message->chip)].id = 1;
            }
            else if(memory->blocks[message->addr].state == 2){
                systemRequest.acction = message->acction;
                systemRequest.addr = message->addr;
                memory->blocks[message->addr].state = 1;
                memory->blocks[message->addr].procs.bit[getProcessorNum(message->id,message->chip)].id = 1;
                return systemRequest; 
            }
            else {
                systemRequest.acction = message->acction;
                systemRequest.addr = message->addr;
                 memory->blocks[message->addr].procs.bit[getProcessorNum(message->id,message->chip)].id = 1;
                return systemRequest; 
            }
            break;
        case 6:
            //Invalidate Caches si esta compartido 
            break;
        case 7:
            //Actualizo memoria nada mas
            break;
        }
    }
}*/