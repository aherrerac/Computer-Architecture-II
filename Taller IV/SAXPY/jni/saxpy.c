/**
 * Computer Engineering, Costa Rica Institute of Technology, Costa Rica
 * CE-4302 – Copmuter Architecture II
 * 
 * @file saxpy.c
 * 
 * @brief This .c file calcs saxpy function with a serial code 
 *
 * @author Jose Andres Rivera Tencio
 * @author Andre Herrera Chacon
 */


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>


#define SIZE1 15000000
#define SIZE2 30000000
#define SIZE3 45000000

float x_1[SIZE1], x_2[SIZE2], x_3[SIZE3];
float y_1[SIZE1], y_2[SIZE2], y_3[SIZE3];




int main ()
{
	float a = .5f;

	int i;

	double start_time, run_time;
	//Fil vectors
	for(i = 0; i<SIZE1; i++){
        x_1[i] = (i+1)*15;
        y_1[i] = (i+1)*.1;
    }
	for(i = 0; i<SIZE2; i++){
        x_2[i] = (i+1)*15;
        y_2[i] = (i+1)*.1;
    }
	for(i = 0; i<SIZE3; i++){
        x_3[i] = (i+1)*15;
        y_3[i] = (i+1)*.1;
    }
	//Serial cal SAXPY
	start_time = omp_get_wtime();
	for(i=0; i <= SIZE1; i++){
            y_1[i]=a*x_1[i]+y_1[i];
    }
	run_time = omp_get_wtime() - start_time;
	printf("SAXPY -> Array size = %d in %f seconds\n",SIZE1,run_time);

	start_time = omp_get_wtime();
	for(i=0; i <= SIZE2; i++){
            y_2[i]=a*x_2[i]+y_2[i];
    }
	run_time = omp_get_wtime() - start_time;
	printf("SAXPY -> Array size = %d in %f seconds\n",SIZE2,run_time);
	

	start_time = omp_get_wtime();
	for(i=0; i <= SIZE3; i++){
            y_3[i]=a*x_3[i]+y_3[i];
    }
	run_time = omp_get_wtime() - start_time;
	printf("SAXPY -> Array size = %d in %f seconds\n",SIZE3,run_time);

	return 0;
}