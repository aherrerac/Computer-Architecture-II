__kernel void vector_add(__global const int *A, __global const int *B, __global int *C) {
 
    // Get the index of the current elements to be processed

    int i = get_global_id(0);
    int j = get_global_id(1);

     
    int value = 0;

    //Perform matrix multiplication divided by segments

    for (int k = 0; k < 4; ++k)
    {
    	value += A[i*4 + k] * B[k*4 + j];
    }

    C[i*4 + j] = value;
}
