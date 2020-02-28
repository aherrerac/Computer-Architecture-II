
/**
 * Computer Engineering, Costa Rica Institute of Technology
 * CE-4302 – Computer Architecture II 
 * 
 * @file Problem_1.c
 * 
 * @brief Application with three threads. Two generates
 * random numbers and stores them in a array, the other does
 * AND function between arrays. 
 * 
 * @author Andre Herrera Chacon
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  
#include <pthread.h> 
#include <time.h>

// A normal C function that is executed as a thread  
// when its name is specified in pthread_create() 
#define SIZE_MAX 1000

//Int Arrays
int a1[SIZE_MAX];
int a2[SIZE_MAX];

int count = 0; 

//Generates random 8bits number
int randNumber()
{
    int num = (rand() % 
    (255 - 0 + 1)) + 0;
    return num; 
}

//Store a number in a1 every 10ms
void *storeInts10() 
{
    int temp = 0;
    while(temp<SIZE_MAX)
    { 
        usleep(10000); 
        a1[temp] = randNumber(); 
        temp++;
    }    
}
//Store a number in a2 every 5ms
void *storeInts5() 
{ 
    int temp = 0;
    while(temp<SIZE_MAX)
    { 
        usleep(5000); 
        a2[temp] = randNumber(); 
        temp++;
    }    
} 
//Executes AND function between a1 and a2 data
void *AND() 
{ 
    int temp = 0;
    while(temp<SIZE_MAX)
    {
        usleep(10000); 
        int and  = a1[temp] && a2[temp];
        printf("Operation result (%d && %d) = %d\n",a1[temp],a2[temp],and);
        temp++; 
    } 
} 
   
int main() 
{ 
    //Defines and creates threads
    pthread_t thread_1;  
    pthread_create(&thread_1, NULL,storeInts10, NULL); 
    pthread_t thread_2; 
    pthread_create(&thread_2, NULL,storeInts5, NULL); 
    pthread_t thread_and; 
    pthread_create(&thread_and, NULL, AND, NULL); 
    //Threads joins
    pthread_join(thread_1, NULL); 
    pthread_join(thread_2, NULL); 
    pthread_join(thread_and, NULL);
    exit(0); 
}
