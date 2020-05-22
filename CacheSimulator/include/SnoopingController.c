//
// Created by aherrerac on 5/5/20.
//

#include "SnoopingController.h"

void proController (struct message *message){
    //TODO: ADD Invalid case
    if(message->request == 1 && message->state == 1 || message->state == 2){
        printf("Read hit \n");
        printf("Stay\n");
    }
    else if(message->request == 2 && message->state == 3){
        //TODO: See when it should change to Shared
        printf("Place read miss on bus \n");
        printf("Ivalid -> Shared\n");
    }
    else if(message->request == 2 && message->state == 1){
        printf("Address conflict miss: place read miss on bus \n");
        printf("Stay in Shared State\n");
    }
    else if(message->request == 2 && message->state == 2){
        printf("Address conflict miss: write-back block, then place read on bus \n");
        printf("Stay in this Exclusive");
    }
    else if(message->request == 3 && message->state == 2){
        printf("Write hit, local cache \n");
        printf("Stay\n");
    }
    else if(message->request == 4 && message->state == 3){
        printf("Place write miss on bus \n");
        printf("Invalid -> shared\n");
    }
    else if(message->request == 3 && message->state == 1){
        printf("Place invalidate on bus. Ownership misses");
        printf("Shared -> Exclusive\n");
    }
    else if(message->request == 4 && message->state == 1){
        printf("Address conflict miss: place read miss on bus \n");
        printf("Shared -> Exclusive\n");

    }
    else if(message->request == 4 && message->state == 2){
        printf("Address conflict miss: write-back block, then place read on bus \n");
        printf("Stay in exclusive \n");
    }
    else{
        printf("Processor Controller: Something gone wrong \n");
    }
}
void busController(struct message *message){
    if(message->request == 2 && message->state ==1){
        printf("Allow shared cache or memory to service read miss \n");
        printf("Stay at Sahred\n");
    }
    else if(message->request == 2 && message->state == 2){
        printf("Attempt to share data: place cache block on bus and change state to shared \n");
        printf("Write back block & abort memory access \n");
    }
    else if(message->request == 5 && message->state == 1){
        printf("Attempt to write shared block: invalidate the block \n");
    }
    else if(message->request == 4 && message->state == 1){
        printf("Attempt to write shared block: invalidate the cache block \n");
    }
    else if(message->request == 4 && message->state == 2){
        printf("Attempt to write block that is exclusive elsewhere \n");
        printf("Write back block & abort memory access \n");
    }
    else{
        printf("Bus Controller: Something gone wrong \n");
    }
}