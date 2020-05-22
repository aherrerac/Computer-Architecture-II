#include<stdio.h>
#include<omp.h>

int main(){ 

    printf("Max number of available threads %d \n", omp_get_max_threads());
    printf("Max number of available processors for %d \n",omp_get_num_procs());

}