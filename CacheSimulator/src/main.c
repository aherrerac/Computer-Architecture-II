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
    
    
    
    
    
    
    //;
    gsl_rng_free (opSeed);
    gsl_rng_free (dirSeed);

    return NULL;
}

void *controller(void * attr){
    //Poner quien gana el acceso jeje 

    //Messages 
    L2message request;
    L2message memoryStatus;

    //Flags 
    int waitingRequest = 0;
    int processInstr = 0;
    int busWriteEnable = 0;
    int waitingbusAccess = 0;
    int waitingPrRequest = 0;
    int waitingBusRequest = 0;

    controllerattr * args = (controllerattr *) attr;

    while (1)
    {
        pthread_mutex_lock(&lock);
        if(busReadCr[args->CoreID]){
            if (waitingbusAccess)
            {
                if (memBus.action == request.acction)
                {   //1
                    //Invalido accion del request
                    //Invalido que los procesadores escriban
                    if (1) // Necesito valor de procesadores
                    {
                       //Escribo
                    //Agrego procesadores a leer 
                    //Espero request
                    }
                    else
                    {
                        //respondo de una vez y
                        //Cambio mi variable de lectura 
                    }

                }
                else
                {
                    //Guardo el proceso que va hacia abajo y
                    //Ejecuto lo de arriba
                    if (1)//Necesito procesadores
                    {
                        /* code */
                    }
                    else
                    {
                        //No necesito
                    }
                }
    
            }
            else{
                if (1)//Requiero procesador)
                {
                    //Hace lo mismo del punto 1
                }
                else{
                    //Realizo el manejo de memoria y devuelvo el dato
                }
                
            }        
        }
        else if (waitingPrRequest)
        {
            if(1){ //Ya me leyeron 
                //Cambio variables de espera 
                //Contesto 
                //Paso mi variable de respuesta a cero
                if(1){ //Es igual al valor que estoy esperando para otro
                    //Actualizo memoria y envio el dato
                }

            }
            else
            {
                //Sigo esperando
            }
        }
        else if(waitingBusRequest){
            if (busReadCr[args->CoreID]){
                //Leo y contesto la peticion
            }
            else
            {
                //Sigo esperando
            }
        }
        else if(1) //Alguien escribio los dos tienen enable en cero
        {
            //Determino si tengo el valor
            if(1){
                //Puedo dar el valor
            }
            else if (1)//El valor le pertenece a otro
            {
                //doy control al procesador y espero
            }
            else{
                //No esta en el chip
                if(1){
                    //puedo en viar la peticcion la envio
                }
                else{
                    //Espero 
                }
            }
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
