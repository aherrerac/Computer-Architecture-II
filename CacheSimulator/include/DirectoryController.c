//
// Created by aherrerac on 5/5/20.
//

#include "DirectoryController.h"

void localDirController(struct nodeMessage * message){
    //TODO: Agregar comparacion para ver si le envia Message Contents
    if(message->type == 1){
        printf("Request data and make P a read sharer \n");
    }
    else if(message->type == 2){
        printf("Request data and make P the exclusive owner \n");
    }
    else if(message->type == 3){
        printf("Request to invalidates to all remote nodes \n");
    }
    else{
        printf("Local cache: Something gone wrong \n");
    }
}
void homeDirController(struct nodeMessage * message){
    if(message->type == 3){
        printf("Invalidate a shared copy of data at Address A \n");
    }
    else if(message->type == 4){
        printf("Fetch the block,change the state \n");
    }
    else if(message->type == 5){
        printf("Fetch the block,invalidate the block \n");
    }
    else if(message->type == 6){
        printf("Return data value from the home \n");
    }
    else{
        printf("Home directory: Something gone wrong \n");
    }
}
void remoteDirController(struct nodeMessage * message){
    printf("Write-back data value for address \n ");
}

void directoryController(struct nodeMessage * message){
    if(message->state == 1 && message->type == 1){
        printf("Data value reply; Sharers = {P}\n");
        printf("Uncached -> Shared\n");
    }
    else if(message->state == 1 && message->type == 2){
        printf("Data value reply; Sharers = {P} \n");
        printf("Uncached -> Exclusive\n");
    }
    else if(message->state == 2 && message->type == 1){
        printf("Data value reply; Sharers += {P}\n");
        printf("Stay\n");
    }
    else if(message->state == 2 && message->type == 2){
        printf("Data value reply; Sharers += {P}\n");
        printf("Shared -> Exclusive\n");
    }
    else if(message->state == 3 && message->type == 1){
        printf("Fetch ;Data value reply; Sharers += {P}\n");
        printf("Exclusive -> Shared\n");
    }
     else if(message->state == 3 && message->type == 2){
        printf("Fetch/Invalidate data value reply; Sharers= {P}\n");
        printf("Stay\n");
    }
      else if(message->state == 3 && message->type == 7){
        printf("Sharers= {}\n");
        printf("Exclusive -> Uncached\n");
    }
     else{
        printf("Home directory: Something gone wrong \n");
    }


}
