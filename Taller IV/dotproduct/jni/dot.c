/**
 * Computer Engineering, Costa Rica Institute of Technology, Costa Rica
 * CE-4302 – Copmuter Architecture II
 * 
 * @file dot.c
 * 
 * @brief This .c file calcs dot product function with serial and parallel codes for Android devices 
 * 
 * @author Jose Andres Rivera Tencio
 * @author Andre Herrera Chacon
 */

#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h> 
#include <omp.h>
#include <time.h>

#define size 50000000

//Serial arrays
float x[size], y[size];
//Arrays for intrinsics ARM(NEON)
float32_t xp[size], yp[size]; 


float a = 0;
//Intrinsics
float32x4_t ap;


int main (){
	
	//ARM type
	ap = vdupq_n_f32(0.5f);
	
	double start_time, run_time;
	srand(time(NULL));
	//Get number of processors
	int procs = omp_get_num_procs();
	//Set threads
	omp_set_num_threads(procs);

	for(int i = 0; i<size; i++){
        x[i] = xp[i] = (i+1)*15;
        y[i] = yp[i] =( i+1)*.1;
    }

	start_time = omp_get_wtime();
	float temp = 0;
	for (int i = 0; i < size; ++i)
	{
		temp = x[i]*y[i]; 
		a += temp;
	}
	run_time = omp_get_wtime() - start_time;
	printf("Not OMP Array size = %d in %f seconds\n",size, run_time);


	//Dot Product OMP & NEON
	start_time = omp_get_wtime();
	#pragma omp parallel  
	{
		float32x4_t temp = vdupq_n_f32(0);
		#pragma omp for private(temp)
		for (int i = 0; i < size; i+=4)
		{
			float32x4_t y_vec = vld1q_f32(yp+i); 
			float32x4_t x_vec = vld1q_f32(xp+i); 
			//Intrinsics mul 
			temp = vmulq_f32(x_vec, y_vec);
			//Intrinsics
			ap = vaddq_f32(temp, ap);
		}
	}
	
	a = ap[0] + ap[1] + ap[2] + ap[3];
	run_time = omp_get_wtime() - start_time;
	printf("OMP + NEON Array size = %d in %f seconds\n",size, run_time);
	return 0;
}