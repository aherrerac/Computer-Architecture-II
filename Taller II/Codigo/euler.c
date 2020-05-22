#include <stdio.h>
#include <omp.h>

static long num_steps = 10000000;
double step;
int main ()
{
	  int i;
	  double sum = 0.0;
      double fact = 1.0;
	  double start_time, run_time;

	  step = 1.0/(double) num_steps;
	 for (i=1;i<=4;i++){
          sum = 1.0;
          fact = 1.0;
          omp_set_num_threads(i);
	  start_time = omp_get_wtime();
#pragma omp parallel  
{
#pragma omp single
	  printf(" num_threads = %d",omp_get_num_threads());

#pragma omp for reduction(+:sum) reduction(*:fact)
	  for (i=1;i<= num_steps; i++){
          fact *= i;
		  sum = sum + (1/fact);
	  }
}
	  run_time = omp_get_wtime() - start_time;
	  printf("\n e is %0.5f in %f seconds and %d threads\n",sum,run_time,i);
}
}	  
