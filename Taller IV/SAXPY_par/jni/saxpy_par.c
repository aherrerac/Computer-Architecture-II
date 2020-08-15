/**
 * Computer Engineering, Costa Rica Institute of Technology, Costa Rica
 * CE-4302 – Copmuter Architecture II
 * 
 * @file saxpy.c
 * 
 * @brief This .c file calcs saxpy function with serial and parallel codes for Android devices 
 * 
 * @author Jose Andres Rivera Tencio
 * @author Andre Herrera Chacon
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <arm_neon.h>
#include <time.h>

#define SIZE1 10000000
#define SIZE2 15000000
#define SIZE3 30000000

float32_t y_1[SIZE1], y_2[SIZE2], y_3[SIZE3];
float32_t x_1[SIZE1], x_2[SIZE2], x_3[SIZE3];

float32x4_t a;

int main()
{
	//Set number of processors available
	omp_set_num_threads(omp_get_num_procs());

	
	//Intrinsics set a float
	a = vdupq_n_f32(.5f);

	int i;

	double start_time, run_time;

	//Fill vectors
	for (i = 0; i < SIZE1; i++)
	{
		x_1[i] = (i + 1) * 15;
		y_1[i] = (i + 1) * .1;
	}

	for (i = 0; i < SIZE2; i++)
	{
		x_2[i] = (i + 1) * 15;
		y_2[i] = (i + 1) * .1;
	}

	for (i = 0; i < SIZE3; i++)
	{
		x_3[i] = (i + 1) * 15;
		y_3[i] = (i + 1) * .1;
	}

	start_time = omp_get_wtime();
	//OMP parallel region 
#pragma omp parallel
	{

#pragma omp for
	//OMP for region
		for (int i = 0; i < SIZE1; i += 4)
		{
			//Create a temp variable 
			float32x4_t ytemp = vld1q_f32(y_1 + i); //Load
			float32x4_t xtemp = vld1q_f32(x_1 + i); //Load
			ytemp = vmlaq_f32(ytemp, a, xtemp); //Mul
			vst1q_f32(y_1 + i, ytemp); //Set
		}
	}
	run_time = omp_get_wtime() - start_time;
	printf("SAXPY -> Array size = %d in %f seconds \n", SIZE1, run_time);

	start_time = omp_get_wtime();

#pragma omp parallel
	{

#pragma omp for
		for (int i = 0; i < SIZE2; i += 4)
		{
			float32x4_t ytemp = vld1q_f32(y_2 + i); //Load
			float32x4_t xtemp = vld1q_f32(x_2 + i);	//Load
			ytemp = vmlaq_f32(ytemp, a, xtemp);	//Mul
			vst1q_f32(y_2 + i, ytemp); //Set
		}
	}
	run_time = omp_get_wtime() - start_time;
	printf("SAXPY -> Array size = %d in %f seconds \n", SIZE2, run_time);

	start_time = omp_get_wtime();

#pragma omp parallel
	{
#pragma omp for
		for (int i = 0; i < SIZE3; i += 4)
		{
			float32x4_t ytemp = vld1q_f32(y_3 + i); //Load
			float32x4_t xtemp = vld1q_f32(x_3 + i); //Load
			ytemp = vmlaq_f32(ytemp, a, xtemp);	//Mul
			vst1q_f32(y_3 + i, ytemp); //Set
		}
	}
	run_time = omp_get_wtime() - start_time;
	printf("SAXPY -> Array size = %d in %f seconds \n", SIZE3, run_time);

	return 0;
}