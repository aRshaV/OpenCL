# OpenCL

### OpenCL examples

* Vector Addition
* Histogram
 _____
 
## Main Steps to execute a simple OpenCL application

**1. Discovering the platform and devices**
> **NOTE:**
> This code provides all platforms and devices, these will answer to function calls

```c
cl_int status; //used for error checking

// Retrieve the number of platforms
cl_uint numPlatforms = 0;
status = clGetPlatformIDs(0, NULL, &numPlatforms);

// Allocate enough space for each platform
cl_platform_id *platforms = NULL;
platforms = (cl_platform_id*) malloc(numPlatforms*sizeof(cl_platform_id));

// Fill in the platforms
status = clGetPlatformIDs(numPlatforms, platforms, NULL);

//Retrieve the number of devices
cl_uint numDevices = 0;
status = clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);

// Allocate enough space for each device
cl_device_id *devices;
devices = (cl_device_id*) malloc(numDevices*sizeof(cl_device_id));

//Fill in the device
status = clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
```

**2. Creating Context**

```c
// Create a context that includes all devices
cl_context context = clCreateContext(NULL, numDevices, devices, NULL, NULL, &status);

```

**3. Creating command queue per device**
> **NOTE:**
> Command-queue only for device 0 

```c
// Only create a command-queue for the 1st device
cl_command_queue cmdQueue = clCreateCommandQueueWithProperties(context, devices[0], &status);
```

**4. Creating Buffers to hold data**
> **NOTE:**
> Needs the size of the buffer, the context (visible to all devices within this). **Optional Flags** W,R and R/W
> NULL 4th argument represents uninitialized

```c
// Allocate 2 input and 1 output buffer for the three vectors in vector addition
cl_mem bufA =clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, &status);
cl_mem bufB =clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, &status);
cl_mem bufC =clCreateBuffer(context, CL_MEM_WRITE_ONLY, datasize, NULL, &status);
```

**5. Copying the input data onto the device**
> **NOTE:**
> Copy data from host pointer to buffer. **CL_TRUE Flag** ensure that data is copied before the API call returns

```c
// Write data from the input arrays to the buffers
status = clEnqueueWriteBuffer(cmdQueue, bufA, CL_TRUE, 0, datasize, A, 0, NULL, NULL);
status = clEnqueueWriteBuffer(cmdQueue, bufB, CL_TRUE, 0, datasize, B, 0, NULL, NULL);
```

**6. Creating and compiling a program from the OpenCL C source code**
> **NOTE:**
> For VecAddition example source code is in a character array programSource

```c
// Create a program with source code
cl_program program = clCreateProgramWithSource(context, 1, (const char**) &programSource, NULL, &status);

// Build (compile) the program for the device
status = clBuildProgram(program, numDevices, devices, NULL, NULL, NULL);
```

**7. Extracting the kernel from the program**
> **NOTE:**
> Kernel is created by selecting the desired function within the program

```c
// Create the vector addition kernel
cl_kernel kernel = clCreateKernel(program, "vecadd", &status);
```

**8. Executing the kernel from the program**
```c
// Set the kernel Arguments
status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);

// Define an index space of work-items for execution.
// A work-group size is not required, but can be used.

size_t indexSpaceSize[1]. workGroupSize[1];

indexSpaceSize[0] = datasize/sizeof(int);
workGroupSize[0] = 256;

// Execute the kernel for execution
status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, indexSpaceSize, workGroupSize, 0, NULL, NULL);
```

**9. Copying output data back to the host**
```c
// Read the device output buffer to the host output array
status = clEnqueueReadBuffer(cmdQueue, bufC, CL_TRUE, 0, datasize, C, 0, NULL, NULL);
```

**10. Release Resources**
```c
clReleaseKernel(kernel);
clReleaseProgram(program);
clReleaseCommandQueue(cmdQueue);
clReleaseMemObject(bufA);
clReleaseMemObject(bufB);
clReleaseMemObject(bufC);
clReleaseContext(context);

```
