#include <stdio.h>
#include <omp.h>
int main()
{
    static long num_steps = 1000000;
    
    float a = .5f;
    float sum = 0.0f;
    float x[num_steps],y[num_steps];
    int i;
    double start_time, run_time;
    
    for(i = 0; i<num_steps; i++){
        x[i] = (i+1)*15;
        y[i] = (i+1)*.1;
    }
    
    for(i=1;i<=4;i++){
        omp_set_num_threads(i);
        start_time = omp_get_wtime();
#pragma omp parallel shared( x ,y , num_steps, a ) private(i)
{
#pragma omp single
    printf(" num_threads = %d",omp_get_num_threads());
#pragma omp for
    for(i=0; i <= num_steps; i++){
            y[i]=a*x[i]+y[i];
    }
}
    run_time = omp_get_wtime() - start_time;
    printf("\n %f seconds and %d threads\n",run_time,i);
    }
}