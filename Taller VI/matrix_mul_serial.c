/**
 * Computer Engineering, Costa Rica Institute of Technology
 * CE-4302 – Computer Architecture II 
 * 
 * @file matrix_mul
 * 
 * @brief  Performs Matrix multiplications 
 * 
 * @author Andre Herrera Chacon
 *
 * Adapted from  U.S. Department of Energy by Lawrence Livermore National Laboratory
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_ROWS  600		
#define MATRIX_CLMNS 600			

int main(int argc, char *argv[])
{

double a[MATRIX_ROWS][MATRIX_CLMNS], 		
       b[MATRIX_CLMNS][MATRIX_CLMNS],      
       c[MATRIX_CLMNS][MATRIX_ROWS];		


printf("Initializing matrices...\n");
//Initializing 
clock_t t;
t = clock(); 
 for (int i=0; i<MATRIX_CLMNS; i++){
         for (int j=0; j<MATRIX_ROWS; j++){
            a[i][j]= (i+1)+(j+1);
            b[i][j]= (i+1)*(j+1);
         }
      }

//Print Matrix A
      printf("\n Matrix A:");
      for (int i=0; i<MATRIX_CLMNS; i++)
      {
         printf("\n"); 
         for (int j=0; j<MATRIX_ROWS; j++) 
            printf("%6.2f   ", a[i][j]);
      }
      //Print Matrix B
      printf("\n Matrix B:");
      for (int i=0; i<MATRIX_CLMNS; i++)
      {
         printf("\n"); 
         for (int j=0; j<MATRIX_ROWS; j++) 
            printf("%6.2f   ", b[i][j]);
      }
      printf ("\n\n");

//Matrix multiplication
printf("Performing matrix multiply...\n");
for(int i=0;i<MATRIX_ROWS;i++)
   for(int j=0;j<MATRIX_CLMNS;j++)
      for(int k=0;k<MATRIX_CLMNS;k++)
         c[i][j]+= a[i][k] * b[k][j];

printf("Result Matrix:");
for (int i=0; i<MATRIX_ROWS; i++) { 
   printf("\n"); 
   for (int j=0; j<MATRIX_CLMNS; j++) 
      printf("%6.2f   ", c[i][j]);
   }

 t = clock() - t; 
 double time_taken = ((double)t)/CLOCKS_PER_SEC;
 printf("\nExecution time:  %fs \n", time_taken); 
}
