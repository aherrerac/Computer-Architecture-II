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

int enable = 0;

int snoopflag[2];

typedef struct
{
    int coreID;
    int chipID;
   // bus * busDir;
} thredattr;

pthread_mutex_t lock;


void *controller(void * attr){
    return NULL;
}

bus

void *processor(void * attr){
    int waitFlag = 0;

    thredattr * args = (thredattr *) attr;
    instruction  instr; // = (instruction *) malloc(sizeof(instruction));
    
     //Setup random seeds for poisson distribution generator GNU Scientific Library 
    const gsl_rng_type * T;
    gsl_rng * opSeed; //Generates random instructions for processor 00 
    gsl_rng * dirSeed; //Generates random direction for processor 00

    gsl_rng_env_setup();
    T = gsl_rng_default;
    //Initialice random seeds
    opSeed = gsl_rng_alloc (T);
    dirSeed = gsl_rng_alloc (T);
    
    //Set different seed to maintain poisson distribution in each random call 
    gsl_rng_set(opSeed,rand());
    gsl_rng_set(dirSeed,rand());

    L1 L1mem;
    L1mem.l1Blocks[0].address = 10;
    L1mem.l1Blocks[0].state = 0;
    L1mem.l1Blocks[0].address = 8;
    L1mem.l1Blocks[0].state = 0;
    

    while(1)
    {
        pthread_mutex_lock(&lock);

        if(snoopflag == 1){

        }
        
        if(waitFlag == 0){
             intGenPoisson(&instr,opSeed,dirSeed,args->coreID,args->chipID);
        }
       
        printf("Processor %d Chip %d Operation %d Address %d Data %d \n",instr.core,instr.chip,instr.op,instr.address,instr.data);
        
        if(instr.op == 1){
            //printf("CALC Processor %d  Chip %d\n",args->coreID,args->chipID);
            pthread_mutex_unlock(&lock);
            sleep(10);
        }
        else
        {
            privateMessage L1message = prcacheController(&instr,&L1mem);
            if (L1message.acction == 0)
            {
                //printf("Accion con exito \n");
                sleep(10);
                pthread_mutex_unlock(&lock);
            }
            else
            {
                printf("Message Address %d  Operation  %d \n",L1message.addr,L1message.acction);
                pthread_mutex_unlock(&lock);
                //Aqui sigue la mauren
                sleep(10);
            }
        }
    }
   
    gsl_rng_free (opSeed);
    gsl_rng_free (dirSeed);

    return NULL;
}



int main(){

    snoopflag[0] = 1;
    snoopflag[1] = 1;
    srand(time(0)); 
    
    pthread_t tid[2];

    //instruction * iL1;

    //bus chip0Bus;

    thredattr  P00;
    P00.chipID = 0;
    P00.coreID = 0;
    //P00.busDir = &chip0Bus;
    

    thredattr P01 ;
    P01.chipID = 0;
    P01.coreID = 1;
    //P01.busDir = &chip0Bus;

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
