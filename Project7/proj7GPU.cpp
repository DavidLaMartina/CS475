/* David LaMartina
 * lamartid@oregonstate.edu
 * Project 7B: Autocorrelation using CPU OpenMP, CPU SIMD, and GPU (CUDA)
 * CS475 Spr2019
 * Due June 10, 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

#include "CL/cl.h"
#include "CL/cl_platform.h"

#ifndef LOCAL_SIZE
#define LOCAL_SIZE  64
#endif

#ifndef WRITENUMS
#define WRITENUMS   512
#endif

#ifndef CL_FILE
#define CL_FILE     "autoC.cl"
#endif

int     Size;
float*  hArray;
float*  hSums;
FILE*   fp;

void    Wait( cl_command_queue );

void readData( )
{
    // Open file and read in size first
    fp = fopen( "signal.txt", "r" );
    if( fp == NULL )
    {
        fprintf( stderr, "Cannot open file 'signal.txt'\n" );
        exit( 1 );
    }
    fscanf( fp, "%d", &Size );
    hArray = (float*)malloc( 2 * Size * sizeof( float ) );
    hSums  = (float*)malloc( 1 * Size * sizeof( float ) );

    // Scan in and duplicate the array to produce "wraparound" effect
    for( int i = 0; i < Size; i++ )
    {
        fscanf( fp, "%f", &hArray[i] );
        hArray[ i + Size ] = hArray[i];
    }
    fclose( fp );
}

void writeData( )
{
    fp = fopen( "resultsGPU.txt", "w" );
    if( fp == NULL )
    {
        fprintf( stderr, "Cannot open file 'resultsGPU.txt' for writing\n" );
        exit( 1 );
    }
    for( int i = 0; i < WRITENUMS; i++ )
    {
        fprintf( fp, "%f\n", hSums[ i ] );
    }
    fclose( fp );
}

int main( int argc, char* argv[] )
{
    // Try to open openCL kernel program
    FILE* fp = fopen( "autoC.cl", "r" );
    if( fp == NULL ){
        fprintf( stderr, "Cannot open OpenCL source file. Exiting.\n" );
        return 1;
    }
    
    cl_int status;      // return status from OpenCL calls

    // Get platform id:
    cl_platform_id platform;
    status = clGetPlatformIDs( 1, &platform, NULL );
    if( status != CL_SUCCESS )
        fprintf( stderr, "clGetDeviceIDs failed (2)\n" );

    // Get device id:
    cl_device_id device;
    status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    if( status != CL_SUCCESS)
        fprintf( stderr, "clGetDeviceIDs failed (2)\n" );

    // Allocate & fill host memory buffers
    readData();

    // Create openCL context
    cl_context context = clCreateContext( NULL, 1, &device, NULL, NULL, &status);
    if( status != CL_SUCCESS )
        fprintf( stderr, "clCreateContext failed\n" );

    // Create openCL command queue
    cl_command_queue cmdQueue = clCreateCommandQueue( context, device, 0, &status );
    if( status != CL_SUCCESS )
        fprintf( stderr, "clCreateCommandQueue failed\n" );

    // Allocate device mem buffers
    cl_mem dArray = clCreateBuffer( context, CL_MEM_READ_ONLY,  2*Size*sizeof(cl_float), NULL, &status );
    if( status != CL_SUCCESS )
        fprintf( stderr, "clCreateBuffer failed (1)\n" );
    cl_mem dSums  = clCreateBuffer( context, CL_MEM_WRITE_ONLY, 1*Size*sizeof(cl_float), NULL, &status );
    if( status != CL_SUCCESS )
        fprintf( stderr, "clCreateBuffer failed (2)\n" );

    // Enqueue commands to write data from host buffer into device buffers
    status = clEnqueueWriteBuffer( cmdQueue, dArray, CL_FALSE, 0, 2*Size*sizeof(cl_float), hArray, 0, NULL, NULL );
    if( status != CL_SUCCESS )
        fprintf( stderr, "clEnqueueWriteBuffer failed (1)\n" );
    status = clEnqueueWriteBuffer( cmdQueue, dSums,  CL_FALSE, 0, 1*Size*sizeof(cl_float), hSums,  0, NULL, NULL );

    // Read kernel code from file
    fseek( fp, 0, SEEK_END );
    size_t fileSize = ftell( fp );
    fseek( fp, 0, SEEK_SET );
    char *clProgramText = new char[ fileSize + 1 ];
    size_t n = fread( clProgramText, 1, fileSize, fp );
    clProgramText[ fileSize ] = '\0';
    fclose( fp );
    if( n != fileSize )
        fprintf( stderr, "Expected to read %d bytes from '%s' -- actually read %d.\n", fileSize, "autoC.cl", n );

    // Create text for kernel program
    char* strings[1];
    strings[0] = clProgramText;
    cl_program program = clCreateProgramWithSource( context, 1, (const char**)strings, NULL, &status );
    if( status != CL_SUCCESS )
        fprintf( stderr, "clCreateProgramWithSource failed\n" );
    delete[] clProgramText;

    // Compile and link kernel code
    char *options = { "" };
    status = clBuildProgram( program, 1, &device, options, NULL, NULL );
    if( status != CL_SUCCESS )
    {
        size_t size;
        clGetProgramBuildInfo( program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size );
        cl_char *log = new cl_char[ size ];
        clGetProgramBuildInfo( program, device, CL_PROGRAM_BUILD_LOG, size, log, NULL );
        fprintf( stderr, "clBuildProgram failed:\n%s\n", log );
        delete[] log;
    }

    // Create kernel object
    cl_kernel kernel = clCreateKernel( program, "AutoCorrelate", &status );
    if( status != CL_SUCCESS )
        fprintf( stderr, "clCreateKernel failed\n" );

    // Set arguments to kernel object
    status = clSetKernelArg( kernel, 0, sizeof(cl_mem), &dArray );
    if( status != CL_SUCCESS )
        fprintf( stderr, "clSetKernelArg failed (1)\n" );
    status = clSetKernelArg( kernel, 1, sizeof(cl_mem), &dSums  );
    if( status != CL_SUCCESS )
        fprintf( stderr, "clSetKernelArg failed (2)\n" );

    // Enqueue kernel object for execution
    size_t globalWorkSize[3] = { Size,          1, 1 };
    size_t localWorkSize[ 3] = { LOCAL_SIZE,    1, 1 };

    Wait( cmdQueue );
    double time0 = omp_get_wtime();

    status = clEnqueueNDRangeKernel( cmdQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL );
    if( status != CL_SUCCESS )
        fprintf( stderr, "clEnqueueNDRangeKernel failed: %d\n", status );

    Wait( cmdQueue );
    double time1 = omp_get_wtime();

    // Read resultsbuffer back from device to host
    status = clEnqueueReadBuffer( cmdQueue, dSums, CL_TRUE, 0, 1*Size*sizeof(cl_float), hSums, 0, NULL, NULL );
    if( status != CL_SUCCESS )
        fprintf( stderr, "clEnqueueReadBuffer failed\n" );

    double performance = (double)Size / (time1 - time0) / 1000000.;
    printf( "Performance: %.3lf MegaFunc per second\n", performance );

    writeData();

    // Clean up
    free( hArray );
    free( hSums  );

    clReleaseKernel(        kernel      );
    clReleaseProgram(       program     );
    clReleaseCommandQueue(  cmdQueue    );
    clReleaseMemObject(     dArray      );
    clReleaseMemObject(     dSums       );

    return 0;
}

void Wait( cl_command_queue queue )
{
    cl_event    wait;
    cl_int      status;

    status = clEnqueueMarker( queue, &wait );
    if( status != CL_SUCCESS )
        fprintf( stderr, "Wait: clEnqueueMarker failed\n" );

    status = clWaitForEvents( 1, &wait );
    if( status != CL_SUCCESS )
        fprintf( stderr, "Wait: clWaitForEvents failed\n" );
}
