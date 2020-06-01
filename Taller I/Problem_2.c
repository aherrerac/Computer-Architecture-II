
/**
 * Computer Engineering, Costa Rica Institute of Technology
 * CE-4302 – Computer Architecture II 
 * 
 * @file Problem_2.c
 * 
 * @brief Mutex example of two threads doing the same task,
 * incrementing a counter 
 * 
 * @author Andre Herrera Chacon
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  
#include <pthread.h> 
#include <time.h>

//Global counter
int count = 0; 

pthread_mutex_t lock;

//Threads 
pthread_t thread_A; 
pthread_t thread_B; 

//Increases counter variable
void *raceProcessA()
{
    while(count<100)
    { 
        printf("Executing process A\n"); 
        pthread_mutex_lock(&lock);
        count++;
        pthread_mutex_unlock(&lock);
        printf("End process A\n"); 
        sleep(10); 
    }    
}
//Increases counter variable
void *raceProcessB()
{
    while(count<100)
    { 
        printf("Executing process B\n"); 
        pthread_mutex_lock(&lock);
        count++;
        pthread_mutex_unlock(&lock);
        printf("End process B\n"); 
        sleep(1); 
    }    
}
   
int main() 
{ 
    //Create threads
    pthread_create(&thread_A, NULL, raceProcessA, NULL);
    pthread_create(&thread_B, NULL, raceProcessB, NULL);
    //Join threads
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL); 
    //Destroy Mutex
    pthread_mutex_destroy(&lock);
   
    exit(0); 
}
