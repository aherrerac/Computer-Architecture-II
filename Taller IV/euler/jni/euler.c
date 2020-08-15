/**
 * Computer Engineering, Costa Rica Institute of Technology, Costa Rica
 * CE-4302 – Copmuter Architecture II
 * 
 * @file euler.c
 * 
 * @brief This .c file calcs euler variable with serial and parallel codes 
 * 
 * @author Jose Andres Rivera Tencio
 * @author Andre Herrera Chacon
 */

#include <stdio.h>
#include <omp.h>

static long num_steps[6] = {10000000, 20000000, 30000000, 40000000, 50000000, 60000000};
double step;
int main()
{

	int i;
	double sum = 0.0;
	double fact = 1.0;
	double start_time, run_time;
	//Get processors available
	int procs = omp_get_num_procs();
	omp_set_num_threads(procs);

	int k;
	for (k = 0; k < 6; k++)
	{

		step = 1.0 / (double)num_steps[k];

		sum = 1.0;
		fact = 1.0;
		start_time = omp_get_wtime();
		for (i = 1; i <= num_steps[k]; i++)
		{
			fact *= i;
			sum = sum + (1 / fact);
		}
		run_time = omp_get_wtime() - start_time;
		printf("\n Serial -> e is %0.5f in %f seconds array size = %ld \n", sum, run_time, num_steps[k]);

		sum = 1.0;
		fact = 1.0;

		start_time = omp_get_wtime();
		//OMP parallel section
#pragma omp parallel
		{
			//OMP for reduction
#pragma omp for reduction(+                  \
						  : sum) reduction(* \
										   : fact)
			for (i = 1; i <= num_steps[k]; i++)
			{
				fact *= i;
				sum = sum + (1 / fact);
			}
		}
		run_time = omp_get_wtime() - start_time;
		printf("\n Parallel -> e is %0.5f in %f seconds array size = %ld \n", sum, run_time, num_steps[k]);
	}
}
