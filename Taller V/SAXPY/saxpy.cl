__kernel void vector_add(__global const int *A, __global const int *B, __global int *C) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 
    //SAXPY Operation C = A * X[i] + Y[i]
    C[i] = (*A) * B[i] + C[i];
}