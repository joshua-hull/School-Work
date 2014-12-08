//
// Hello World for OpenCL - Fall, 2013.
// This just adds two vectors of length 5, but it's really about the calling
// syntax, which is explained in the comments.
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <CL/cl_ext.h>
#include <CL/cl_gl_ext.h>
#include <CL/cl_platform.h>
#include <CL/opencl.h>
#include "RGU.h"

#define N 5

int vector1[N] = {0,1,2,3,4};
int vector2[N] = {5,6,7,8,9};
int outv[N];
size_t work[1] = {N};

int main(int argc, char** argv)
{
cl_platform_id myplatform;
cl_context mycontext;
cl_device_id *mydevice;
cl_command_queue mycommandq;
cl_kernel mykernelfunc;
cl_program myprogram;
cl_mem gpuv_in1, gpuv_in2, gpuv_out;
int err;
unsigned int gpudevcount;

size_t mycontxtsize;		// unsigned long (64 bits) 
size_t kernelsize;
char *kernelsource;
size_t i;

// RGU.h contains Robert Geist's Utilities, a rewrite of OCLutilities.h for
// use with CUDA 5 based OpenGL.
err = RGUGetPlatformID(&myplatform);

// get number of GPU devices available on this platform:
err = clGetDeviceIDs(myplatform,CL_DEVICE_TYPE_GPU,0,NULL,&gpudevcount);

// create the device list:
mydevice = new cl_device_id[gpudevcount];
err = clGetDeviceIDs(myplatform,CL_DEVICE_TYPE_GPU,gpudevcount,mydevice,NULL);

cl_context_properties props[] = {CL_CONTEXT_PLATFORM, 
	(cl_context_properties)myplatform, 0};
mycontext = clCreateContext(props,1,&mydevice[0],NULL,NULL,&err);
mycommandq = clCreateCommandQueue(mycontext,mydevice[0],0,&err);

// create buffer object == allocate mem on device
// RO means RO from a kernel; CL_MEM_COPY_HOST_PTR: alloc device memory 
// and copy data referenced by the host pointer; 
// arg4: error code return
gpuv_in1 = clCreateBuffer(mycontext,CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
	N*sizeof(int),vector1,NULL);
gpuv_in2 = clCreateBuffer(mycontext,CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
	N*sizeof(int),vector2,NULL);

// WO: written but not read from a kernel
gpuv_out = clCreateBuffer(mycontext,CL_MEM_WRITE_ONLY,N*sizeof(int),NULL,NULL);

// arg0: file name of kernel to load
// arg1: preamble to prepend, e.g., .h file
// arg2: final length of code 
kernelsource = RGULoadProgSource("myvadd.cl","", &kernelsize);

// creates program object and loads source strings into it
// arg1: number of string pointers in array of string pointers
// arg2: array of string pointers
// arg3: array of size_ts with lengths of strings; 
//	 NULL==(all strings null-terminated)
// arg4: error code return
myprogram = clCreateProgramWithSource(mycontext,1,
	(const char **)&kernelsource, NULL, NULL);

// compiles and links for all devices in context
// arg1: number of devices in device list
// arg2: device list ptr; NULL == (use all devices in context)
// arg3: compile options; see section 5.4.3 for listing
// arg4: callback function; called when compilation done; if NULL, suspend until
// arg5: data to callback function
clBuildProgram(myprogram,0,NULL,NULL,NULL,NULL);

// create kernel object
// arg1: kernel function name
// arg2: error code
mykernelfunc = clCreateKernel(myprogram,"myvadd",NULL);

// arg1: which argument (L-to-R)
// arg2: size of argument; can use sizeof(type) for mem objects
// arg3: argument *
clSetKernelArg(mykernelfunc,0,sizeof(cl_mem),(void *)&gpuv_out);
clSetKernelArg(mykernelfunc,1,sizeof(cl_mem),(void *)&gpuv_in1);
clSetKernelArg(mykernelfunc,2,sizeof(cl_mem),(void *)&gpuv_in2);

// arg2: work dimension
// arg3: must be NULL; will be global work id offsets, instead of (0,0,...0)
// arg4: array of work dimension values giving number of work items in each
//       dim that will exec the kernel
// arg5: local work size - array of work dimension values giving work group
//       size in each dim; NULL = (OpenCL decides on work group sizes; 
//       Danger Will Robinson! OpenCL will make *BAD* decisions on this!)
// arg6: number of events in event waitlist
// arg7: event waitlist ... commands that must complete before exec this one
// arg8: event ... returns event object that identifies this kernel execution
//       instance; event objects are unique
clEnqueueNDRangeKernel(mycommandq,mykernelfunc,1,NULL,work,NULL,0,NULL,NULL);

// read from a buffer object to host mem
// arg1: buffer object
// arg2: blocking read
// arg3: offset
// arg4: size in bytes
// arg5: host ptr
// arg6: number of events in event waitlist
// arg7: event waitlist ... commands that must complete before exec this one
// arg8: event ... returns event object that identifies this kernel execution
//       instance; event objects are unique
clEnqueueReadBuffer(mycommandq,gpuv_out,CL_TRUE,0,N*sizeof(int),outv,0,NULL,NULL); 

clReleaseMemObject(gpuv_in1);
clReleaseMemObject(gpuv_in2);
clReleaseMemObject(gpuv_out);
free(mydevice);

for(i=0;i<N;i++) printf("%d ",outv[i]);
printf("\n");
return 0; 
}
