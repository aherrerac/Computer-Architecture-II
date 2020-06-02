#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/instruction.h"
#include "../include/randomPoisson.h"
#include "../include/SnoopingController.h"
#include "../include/message.h"
#include "../include/bus.h"
#include "../include/memory.h"
#include "../include/L2Controller.h"

//Hacer lista de buses;
//Buses
bus chip0Bus;

membus memBus;

typedef struct
{
    int coreID;
    int chipID;
} thredattr;

typedef struct 
{
   int CoreID;
} controllerattr;


int busReadPr[2];

int busReadCr[2];


pthread_mutex_t lock;

void *processor(void * attr){
    //Get processor data
    thredattr * args = (thredattr *) attr;
     //Setup random seeds for poisson distribution generator GNU Scientific Library 
    const gsl_rng_type * T;
    gsl_rng * opSeed; //Generates random instructions for processor 00 
    gsl_rng * dirSeed; //Generates random direction for processor 00
    //Set up gsl 
    gsl_rng_env_setup();
    T = gsl_rng_default;
    //Initialice random seeds
    opSeed = gsl_rng_alloc (T);
    dirSeed = gsl_rng_alloc (T);
    
    //Set different seed to maintain poisson distribution in each random call 
    gsl_rng_set(opSeed,rand());
    gsl_rng_set(dirSeed,rand());

    //Init cache
    L1 L1cache;

    //Messages 
    L1message request;
    L1message memoryStatus;

    //Flags
    int waitingRequest = 0;
    int processInstr = 0;
    int busWriteEnable = 0;
    int waitingbusAccess = 0;
    //Instruction 
    instruction instr;

    while(1){
        pthread_mutex_lock(&lock);
        if(request.addr == chip0Bus.address && busReadPr[args->coreID]){
            if(request.acction != chip0Bus.action){
                //Hacer cambios
                //Cancel previous request
                memoryStatus.acction = chip0Bus.action;
                memoryStatus.addr = chip0Bus.address;
                memoryStatus.data = chip0Bus.data;
                memoryStatus = buscacheController(&memoryStatus,&L1cache);
                
                if (memoryStatus.acction == 3){
                    if(busWriteEnable){
                        chip0Bus.action = memoryStatus.acction;
                        chip0Bus.address = memoryStatus.addr;
                        chip0Bus.data = memoryStatus.data;
                        chip0Bus.id = args->coreID;
                    }
                    else{
                        request = memoryStatus;
                    }
                }
                else{
                    printf("Se realizo el invalidate");
                }  
            }
            else{
                waitingRequest = 0;
                memoryStatus.acction = chip0Bus.action;
                memoryStatus.addr = chip0Bus.address;
                memoryStatus.data = chip0Bus.data;
                memoryStatus = buscacheController(&memoryStatus,&L1cache);
                printf("Se actualizo memoria");
                processInstr = 1;
                //Cambiar el valor de readEnable
            }
        }
        else if (findAddress(chip0Bus.address,&L1cache) && busReadPr[args->coreID])
        {
            if (chip0Bus.action == 3)
            {
                memoryStatus.acction = chip0Bus.action;
                memoryStatus.addr = chip0Bus.address;
                memoryStatus.data = chip0Bus.data;
                memoryStatus = buscacheController(&memoryStatus,&L1cache);
                processInstr = 1;
                if(busWriteEnable){
                    chip0Bus.action = memoryStatus.acction;
                    chip0Bus.address = memoryStatus.addr;
                    chip0Bus.data = memoryStatus.data;
                    chip0Bus.id = args->coreID;
                }
                else{
                    request = memoryStatus;
                    waitingbusAccess = 1;
                }
            }
            else{
                memoryStatus.acction = chip0Bus.action;
                memoryStatus.addr = chip0Bus.address;
                memoryStatus.data = chip0Bus.data;
                memoryStatus = buscacheController(&memoryStatus,&L1cache);
                printf("Invalidated");  
            }
        }
        else if (waitingRequest)
        {
            printf("Processor Waiting");
        }
        
        else if(waitingbusAccess && busWriteEnable){
            chip0Bus.action = request.acction;
            chip0Bus.address = request.addr;
            chip0Bus.data = request.data;
            chip0Bus.id = args->coreID;
            waitingRequest = 1;
        }
        else if (processInstr)
        {
            memoryStatus = prcacheController(&instr,&L1cache); //Agregar que pasa con write back
            if(memoryStatus.acction != 4 || memoryStatus.acction != 5){
                if(busWriteEnable){
                    chip0Bus.action = memoryStatus.acction;
                    chip0Bus.address = memoryStatus.addr;
                    chip0Bus.data = memoryStatus.data;
                    chip0Bus.id = args->coreID;
                }
                else{
                    request = memoryStatus;
                    waitingbusAccess = 1;
                }
            }
            else
            {
                printf("Status");
            }
            
        }
        else
        {
            intGenPoisson(&instr,opSeed,dirSeed,args->coreID,args->chipID);
            memoryStatus = prcacheController(&instr,&L1cache); //agregar que pasa con write back
            if(memoryStatus.acction != 4 || memoryStatus.acction != 5){
                if(busWriteEnable){
                    chip0Bus.action = memoryStatus.acction;
                    chip0Bus.address = memoryStatus.addr;
                    chip0Bus.data = memoryStatus.data;
                    chip0Bus.id = args->coreID;
                }
                else{
                    request = memoryStatus;
                    waitingbusAccess = 1;
                }
            }
            else
            {
                printf("Status");
            }
        }
        pthread_mutex_unlock(&lock);
    }
    gsl_rng_free (opSeed);
    gsl_rng_free (dirSeed);

    return NULL;
}

void *controller(void * attr){
    //Poner quien gana el acceso jeje 

    //Messages 
    L2message request;
    L2message memoryStatus;
    L2message tempMessage;

    //Flags 
    int waitingRequest = 0;
    int processMessage = 0;
    int busWriteEnable = 0;
    int waitingbusAccess = 0;
    int waitingPrRequest = 0;
    int waitingBusRequest = 0;
    int writeEnable[2] = 0;

    int writeEnable[2];
    controllerattr * args = (controllerattr *) attr;

    L2 L2cache;

    while (1)
    {
        pthread_mutex_lock(&lock);
        if(busReadCr[args->CoreID]){
            if (waitingbusAccess)
            {
                if (memBus.action == request.acction)
                {   //Cancelo del todo el request pendiente
                    waitingbusAccess = 0;
                    //Reseteo valores de request de salida del bus
                    writeEnable[0] = 0;
                    writeEnable[1] = 0;
                    memoryStatus.acction = memBus.action;
                    memoryStatus.addr = memBus.address;
                    memoryStatus.data = memBus.data;
                    memoryStatus.sharedExternal = memBus.shared;
                    memoryStatus.id = memBus.id;
                    memoryStatus = buscacheL2Controller(&memoryStatus,&L2cache);
                    if (memoryStatus.shared)
                    {
                        busReadPr[0] = 1;
                        busReadPr[1] = 1;
                        chip0Bus.action = memoryStatus.acction;
                        chip0Bus.address = memoryStatus.addr;
                        chip0Bus.data = memoryStatus.data;
                        chip0Bus.id = memoryStatus.id;
                        busReadCr[args->CoreID] = 0;
                        waitingPrRequest = 1; 
                    }
                    else
                    {
                        busReadPr[memoryStatus.id] = 1;
                        chip0Bus.action = memoryStatus.acction;
                        chip0Bus.address = memoryStatus.addr;
                        chip0Bus.data = memoryStatus.data;
                        chip0Bus.id = memoryStatus.id;
                        busReadCr[args->CoreID] = 0;
                        waitingPrRequest = 1;  
                    }

                }
                else
                {
                    writeEnable[0] = 0;
                    writeEnable[1] = 0;
                    processMessage = 1;
                    tempMessage = request;
                    memoryStatus.acction = memBus.action;
                    memoryStatus.addr = memBus.address;
                    memoryStatus.data = memBus.data;
                    memoryStatus.sharedExternal = memBus.shared;
                    memoryStatus.id = memBus.id;
                    memoryStatus = buscacheL2Controller(&memoryStatus,&L2cache);
                    if (memoryStatus.shared)
                    {
                        busReadPr[0] = 1;
                        busReadPr[1] = 1;
                        chip0Bus.action = memoryStatus.acction;
                        chip0Bus.address = memoryStatus.addr;
                        chip0Bus.data = memoryStatus.data;
                        chip0Bus.id = memoryStatus.id;
                        busReadCr[args->CoreID] = 0; 
                        waitingPrRequest = 1; 
                    }
                    else
                    {
                        busReadPr[memoryStatus.id] = 1;
                        chip0Bus.action = memoryStatus.acction;
                        chip0Bus.address = memoryStatus.addr;
                        chip0Bus.data = memoryStatus.data;
                        chip0Bus.id = memoryStatus.id;
                        busReadCr[args->CoreID] = 0; 
                        waitingPrRequest = 1; 
                    }
                }
    
            } //No pending messages
            else{
                memoryStatus.acction = memBus.action;
                    memoryStatus.addr = memBus.address;
                    memoryStatus.data = memBus.data;
                    memoryStatus.sharedExternal = memBus.shared;
                    memoryStatus.id = memBus.id;
                    memoryStatus = buscacheL2Controller(&memoryStatus,&L2cache);
                    if (memoryStatus.shared)
                    {
                        busReadPr[0] = 1;
                        busReadPr[1] = 1;
                        chip0Bus.action = memoryStatus.acction;
                        chip0Bus.address = memoryStatus.addr;
                        chip0Bus.data = memoryStatus.data;
                        chip0Bus.id = memoryStatus.id;
                        busReadCr[args->CoreID] = 0;
                        waitingPrRequest = 1; 
                    }
                    else
                    {
                        busReadPr[memoryStatus.id] = 1;
                        chip0Bus.action = memoryStatus.acction;
                        chip0Bus.address = memoryStatus.addr;
                        chip0Bus.data = memoryStatus.data;
                        chip0Bus.id = memoryStatus.id;
                        busReadCr[args->CoreID] = 0;
                        waitingPrRequest = 1;  
                    }
            }        
        }
        else if (waitingPrRequest)
        {
            if(memoryStatus.shared){
                if(!busReadPr[0] && !busReadPr[1]){
                    memBus.action = memoryStatus.acction;
                    memBus.address = memoryStatus.addr;
                    memBus.data = memoryStatus.addr;
                    memBus.shared = memoryStatus.sharedExternal;
                    waitingPrRequest = 0;
                }
                else{
                    printf("Waiting Processors");
                }
            }
            else
            {
                if(!busReadPr[memoryStatus.id]){
                    memBus.action = memoryStatus.acction;
                    memBus.address = memoryStatus.addr;
                    memBus.data = memoryStatus.addr;
                    memBus.shared = memoryStatus.sharedExternal;
                    waitingPrRequest = 0;
                }
                else
                {
                    printf("Waiting Processor");
                }
                
            }
        }
        else if(waitingBusRequest){
            if (busReadCr[args->CoreID]){
                memoryStatus.acction = memBus.action;
                memoryStatus.addr = memBus.address;
                memoryStatus.data = memBus.data;
                memoryStatus.sharedExternal = memBus.shared;
                memoryStatus.id = memBus.id;
                memoryStatus = buscacheL2Controller(&memoryStatus,&L2cache);
                busReadPr[1] = 1;
                chip0Bus.action = memoryStatus.acction;
                chip0Bus.address = memoryStatus.addr;
                chip0Bus.data = memoryStatus.data;
                chip0Bus.id = memoryStatus.id;
                busReadCr[args->CoreID] = 0;
                waitingPrRequest = 1; 
            }
            else
            {
                printf("Espero Bus de Memoria");
            }
        }
        else if(!writeEnable[0] && !writeEnable[1]) 
        {
            memoryStatus.acction = chip0Bus.action;
            memoryStatus.addr = chip0Bus.address;
            memoryStatus.id = chip0Bus.id;
            memoryStatus.addr = chip0Bus.address;
            memoryStatus = prcacheL2Controller(&memoryStatus,&L2cache);
            if (memoryStatus.acction == 0 || memoryStatus.acction == 1){
                chip0Bus.action = memoryStatus.acction;
                chip0Bus.address = memoryStatus.addr;
                chip0Bus.data = memoryStatus.data;
                chip0Bus.id = memoryStatus.id;
                busReadCr[args->CoreID] = 0;
                waitingPrRequest = 1; 
            }
            else{
                memBus.action = memoryStatus.acction;
                memBus.address = memoryStatus.addr;
                memBus.data = memoryStatus.addr;
                memBus.shared = memoryStatus.sharedExternal;
                waitingPrRequest = 0;
            }
            
        }
        else if(processMessage){
            request = tempMessage;
            if( 4<= request.acction <= 7){
                if(writeEnable[args->CoreID]){
                     memBus.action = memoryStatus.acction;
                    memBus.address = memoryStatus.addr;
                    memBus.data = memoryStatus.addr;
                    memBus.shared = memoryStatus.sharedExternal;
                    waitingPrRequest = 0;
                }
            } 
            else
            {
                chip0Bus.action = memoryStatus.acction;
                chip0Bus.address = memoryStatus.addr;
                chip0Bus.data = memoryStatus.data;
                chip0Bus.id = memoryStatus.id;
                busReadCr[args->CoreID] = 0;
                waitingPrRequest = 1; 
            }
                   
        } 
        else
        {
            exit(0);
        }
        pthread_mutex_unlock(&lock);
    }
    
}


void *memory(void * attr){

    if(1)//Mensaje pendente
    {
        if(1) {
            }//puedo contestar
        else{
            //paso el control a otro
        }
    }
    else{
        //waiting
    }

}

int main(){

  
    srand(time(0)); 
    
    pthread_t tid[2];

   

    thredattr  P00;
    P00.chipID = 0;
    P00.coreID = 0;
    

    thredattr P01 ;
    P01.chipID = 0;
    P01.coreID = 1;
   

    pthread_create(&tid[0],NULL,processor,&P00);

    pthread_create(&tid[1],NULL,processor,&P01);

    for (int i = 0; i < 2; i++)
    {
        pthread_join(tid[i],NULL);
    }
    
    pthread_mutex_destroy(&lock);
    pthread_exit(NULL);
    return  0;
}
