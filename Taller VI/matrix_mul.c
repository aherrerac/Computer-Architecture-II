/**
 * Computer Engineering, Costa Rica Institute of Technology
 * CE-4302 – Computer Architecture II 
 * 
 * @file matrix_mul
 * 
 * @brief  Performs Matrix multiplications with mpi distributing tasks between workers
 * 
 * @author Andre Herrera Chacon
 *
 * Adapted from  U.S. Department of Energy by Lawrence Livermore National Laboratory
 */

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#define MATRIX_CLMNS 5          
#define MATRIX_ROWS  5                           
#define master_rank 0              
#define FROM_MASTER 1         
#define FROM_WORKER 2         

int main (int argc, char *argv[])
{

int	tasks_num,              //Tasks
	task_id,                //Task Id
	num_workers,            //Num of workers
	source,                //Message source
	dest,                  //Message destination
	mtype,                 //Message Type
	rows,                  //Rows
	averow, extra, offset; //Rows per worker
    
    
double	a[MATRIX_CLMNS][MATRIX_CLMNS],       //Matrix A
	b[MATRIX_CLMNS][MATRIX_CLMNS],           //Matrix B
	c[MATRIX_CLMNS][MATRIX_CLMNS];           //Matrix C -> Result
MPI_Status status;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&task_id);
MPI_Comm_size(MPI_COMM_WORLD,&tasks_num);
num_workers = tasks_num-1;


//Master
   if (task_id == master_rank)
   {
      clock_t t;
      t = clock(); 
      printf("MPI  with %d tasks.\n",tasks_num);
      printf("Initializing arrays...\n");
      for (int i=0; i<MATRIX_CLMNS; i++){
         for (int j=0; j<MATRIX_ROWS; j++){
            a[i][j]= (i+1)+(j+1);
            b[i][j]= (i+1)*(j+1);
         }
      }
      Print Matrix A
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
      printf ("\n\n");*/
           
      //Calc number of rows to each worker
      averow = MATRIX_CLMNS/num_workers;
      extra = MATRIX_CLMNS%num_workers;
      offset = 0;
      mtype = FROM_MASTER;

      //Send data to workers
      for (int dest=1; dest<=num_workers; dest++)
      {
         rows = (dest <= extra) ? averow+1 : averow;   	
         printf("Sending %d rows to task %d\n",rows,dest);
         MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);                    //offset
         MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);                      //rows
         MPI_Send(&a[offset][0], rows*MATRIX_ROWS, MPI_DOUBLE, dest, mtype,             //Matrix A
                   MPI_COMM_WORLD);
         MPI_Send(&b, MATRIX_ROWS*MATRIX_ROWS, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);//Matrix B
         offset = offset + rows;
      }

      //Receive message from workers
      mtype = FROM_WORKER;
      for (int i=1; i<=num_workers; i++)
      {
         source = i;
         MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&c[offset][0], rows*MATRIX_ROWS, MPI_DOUBLE, source, mtype, 
                  MPI_COMM_WORLD, &status);
         printf("Received results from task %d\n",source);
      }

      //Results print
     
      printf("\nResult Matrix:\n");
      for (int i=0; i<MATRIX_CLMNS; i++)
      {
         printf("\n"); 
         for (int j=0; j<MATRIX_ROWS; j++) 
            printf("%6.2f   ", c[i][j]);
      }
      printf ("\n");*/
      t = clock() - t; 
      double time_taken = ((double)t)/CLOCKS_PER_SEC;
      printf("\nExecution time:  %fs \n", time_taken); 
   }

   if (task_id > master_rank)
   {
      mtype = FROM_MASTER;
      MPI_Recv(&offset, 1, MPI_INT, master_rank, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, master_rank, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&a, rows*MATRIX_ROWS, MPI_DOUBLE, master_rank, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&b, MATRIX_ROWS*MATRIX_ROWS, MPI_DOUBLE, master_rank, mtype, MPI_COMM_WORLD, &status);

      char processor_name[MPI_MAX_PROCESSOR_NAME];
      int name_len;
      MPI_Get_processor_name(processor_name, &name_len);
      printf("Hello from %s processor %d of %d\n", processor_name,task_id,tasks_num);

      //Matrix Multiplication

      for (int k=0; k<MATRIX_ROWS; k++)
         for (int i=0; i<rows; i++)
         {
            c[i][k] = 0.0;
            for (int j=0; j<MATRIX_ROWS; j++)
               c[i][k] = c[i][k] + a[i][j] * b[j][k];
         }
      mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, master_rank, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, master_rank, mtype, MPI_COMM_WORLD);
      MPI_Send(&c, rows*MATRIX_ROWS, MPI_DOUBLE, master_rank, mtype, MPI_COMM_WORLD);
   }
   MPI_Finalize();
}
