//
//  main.c
//  VecAddition
//
//  This program implements a vector addition using OpenCL


// System includes
#include <stdio.h>
#include <stdlib.h>
// OpenCL includes
#include <OpenCL/opencl.h>

// OpenCL kernel to perform an element-wise addition
/******* IMPORTANT NOTE *******/
/* For this example the program represents an array of characters but is possible to read it from a file */

const char* programSource =
"__kernel                                           \n"
"void vecadd(  __global int *A,                     \n"
"               __global int *B,                    \n"
"               __global int *C)                    \n"
"{                                                  \n"
"                                                   \n"
" // Get the work-item's unique ID                  \n"
" int idx = get_global_id(0);                       \n"
"                                                   \n"
" // Add the corresponding locations of             \n"
" // 'A' and 'B', and store the result in 'C'.      \n"
" C[idx] = A[idx] + B[idx];                         \n"
"}                                                  \n"
;


int main(int argc, const char * argv[]) {
    // This code executes on the OpenCL host
    
    // Elements in each array
    const int elements = 2048;
    
    // Compute the size of the data
    size_t datasize = sizeof(int)*elements;
    
    // Allocate space for input/output host data
    int *A = (int*)malloc(datasize); // Input array
    int *B = (int*)malloc(datasize); // Input array
    int *C = (int*)malloc(datasize); // Output array
    
    // Initialize the input data
    int i;
    for (i=0; i < elements; i++){
        A[i] = i;
        B[i] = i;
    }
    
    // Use this to check the output of each API call
    cl_int status;
    
    // Get the first platform
    cl_platform_id platform;
    status = clGetPlatformIDs(1, &platform, NULL);
    
    
    cl_uint num_devices;
    clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
    printf("NUM:%d\n",num_devices);
    
    // Get the first device
    cl_device_id* devices;
    devices = (cl_device_id*)malloc(num_devices*sizeof(cl_device_id));
    
    status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 2, devices, NULL);
    
    // Create a context and associate it to the device
    cl_context context = clCreateContext(NULL, 2, devices, NULL, NULL, &status);
    
    // Create a command-queue and associate it with the device
    cl_command_queue cmdQueue = clCreateCommandQueue(context, devices[1], 0, &status);
    
    // Allocate two input buffers and one output buffer for the three vectors in the vector addition
    cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, &status);
    cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, &status);
    cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, datasize, NULL, &status);
    
    // Write data from the input arrays to the buffers
    status = clEnqueueWriteBuffer(cmdQueue, bufA, CL_FALSE, 0, datasize, A, 0, NULL, NULL);
    status = clEnqueueWriteBuffer(cmdQueue, bufB, CL_FALSE, 0, datasize, B, 0, NULL, NULL);
    
    
    // Create program with source code
    cl_program  program = clCreateProgramWithSource(context, 1, (const char**) &programSource, NULL, &status);
    
    
    // Build(compile) the program for the device
    status = clBuildProgram(program, 1, &devices[1], NULL, NULL, NULL);
    
    // Create the vector addition kernel
    cl_kernel kernel = clCreateKernel(program, "vecadd", &status);
    
    // Set the kernel arguments
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);
    
    // Define an index space of work-items for executions.
    // A work-group size is not required, but can be used.
    size_t indexSpace[1], workGroupSize[1];
    
    // There are 'elements' work-items
    indexSpace[0] = elements;
    workGroupSize[0] = 256;
    
    // Execute the kernel
    status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, indexSpace, workGroupSize, 0, NULL, NULL);
    
    // Read the device output buffer to the host output array
    status = clEnqueueReadBuffer(cmdQueue, bufC, CL_TRUE, 0, datasize, C, 0, NULL, NULL);
    
    for(i=0;i < elements;i++)
        printf("C[%d] = %d\n",i,C[i]);
    
    // Free OpenCL resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cmdQueue);
    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);
    clReleaseContext(context);
    
    // Free host resources
    free(A);
    free(B);
    free(C);
    
    return 0;
}
