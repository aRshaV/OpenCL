# OpenCL

### OpenCL examples

* Vector Addition
#### Histogram
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
