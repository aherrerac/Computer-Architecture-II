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

//Bus
bus chip0Bus;

typedef struct
{
    int coreID;
    int chipID;
} thredattr;

int busRead[2];

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
    privateMessage request;
    privateMessage tempRequest;
    privateMessage  memoryStatus;

    //Flags
    int waitingRequest = 0;
    int processInstr = 0;
    int busWriteEnable = 0;
    int waitingbusAccess = 0;
    //Instruction 
    instruction instr;

    while(1){
        pthread_mutex_lock(&lock);
        //Check if address is in L1
        
        if(request.addr == chip0Bus.address && busRead[args->coreID]){
            if(request.acction != chip0Bus.action){
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
            }
        }
        else if (findAddress(chip0Bus.address,&L1cache) && busRead[args->coreID])
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
        else if(waitingbusAccess && busWriteEnable){
            chip0Bus.action = request.acction;
            chip0Bus.address = request.addr;
            chip0Bus.data = request.data;
            chip0Bus.id = args->coreID;
            waitingRequest = 1;
        }
        else if (processInstr)
        {
            memoryStatus = prcacheController(&instr,&L1cache);
        }
        else
        {
            intGenPoisson(&instr,opSeed,dirSeed,args->coreID,args->chipID);
        }
        
        
        
   
   
        pthread_mutex_unlock(&lock);
    }
    
    
    
    
    
    
    //;
    gsl_rng_free (opSeed);
    gsl_rng_free (dirSeed);

    return NULL;
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
