
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
#define SIZE_MAX 5000

int a1[SIZE_MAX];
int a2[SIZE_MAX];

int count = 0; 

int randNumber()
{
    int num = (rand() % 
    (255 - 0 + 1)) + 0;
    return num; 
}

void *myThreadFun(void *vargp) 
{
    int temp = SIZE_MAX;
    while(0<temp)
    { 
        usleep(10000); 
        printf("Printing Thread 1\n"); 
        int r = randNumber();  
        printf("%d\n",r);
        temp--;
    }    
}

void *myThread(void *vargp) 
{ 
    int temp = SIZE_MAX;
    while(0<temp)
    {
        usleep(5000); 
        printf("Printing Thread 2\n");
        int r = randNumber();
        printf("%d\n",r);
        temp--; 
    } 
} 

void *AND(void *vargp) 
{ 
    int temp = 0;
    while(temp<SIZE_MAX)
    {
        usleep(10500); 
        int and  = a1[temp] && a2[temp];
        printf("%d\n AND ",and);
        temp++; 
    } 
} 

void *raceProcessA(void *vargp)
{
    while(count<100)
    { 
        
        printf("Executing process A\n"); 
        count++;
        printf("End process A\n"); 
        sleep(1); 
    }    
}
void *raceProcessB(void *vargp)
{
    while(count<100)
    { 
        printf("Executing process B\n"); 
        count++;
        printf("End process B\n"); 
        sleep(1); 
    }    
}
   
int main() 
{ 
    
   /* pthread_t thread_1;  
    pthread_create(&thread_1, NULL, myThreadFun, NULL); 
    pthread_t thread_2; 
    pthread_create(&thread_2, NULL, myThread, NULL); 
    pthread_t thread_and; 
    pthread_create(&thread_and, NULL, AND, NULL); 
    pthread_join(thread_1, NULL); 
    pthread_join(thread_2, NULL); 
    pthread_join(thread_and, NULL); */
    pthread_t thread_A;  
    pthread_create(&thread_A, NULL, raceProcessA, NULL);
    pthread_t thread_B;  
    pthread_create(&thread_B, NULL, raceProcessB, NULL);
    pthread_join(thread_A, NULL); 
    pthread_join(thread_B, NULL);

    exit(0); 
}
