/* David LaMartina
 * lamartid@oregonstate.edu
 * Project 6: Monte Carlo Simulation with CUDA
 * CS475 Spr2019
 * Adapted from arrayMul.cu provided in CS475
 */

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <cuda_runtime.h>           // Needed to run CUDA code

// Helpers for working with CUDA
#include "helper_functions.h"
#include "helper_cuda.h"

// Parallel constants
#ifndef BLOCKSIZE
#define BLOCKSIZE       32                  // # threads per block
#endif

#ifndef SIZE
#define SIZE            1 * 1024 * 1024     // array size
#endif

#ifndef TOLERANCE
#define TOLERANCE       0.00001f            // tolerance to relative error
#endif

// Ranges
const float XCMIN = 0.0;
const float XCMAX = 2.0;
const float YCMIN = 0.0;
const float YCMAX = 2.0;
const float RMIN =  0.5;
const float RMAX =  2.0;

// Monte Carlo helper prototypes
float Ranf( float, float );
int Ranf( int, int );
void TimeOfDaySeed();

__global__ void MonteCarlo( float* xcs, float* ycs, float* rs, float* rsum )
{
    unsigned int numItems = blockDim.x;
    unsigned int tnum     = threadIdx.x;
    unsigned int wgNum    = blockIdx.x;
    unsigned int gid      = blockIdx.x * blockDim.x * threadIdx.x;

    // Do Monte Carlo calculation
}

int
main( int argc, char* argv[] )
{
    TimeOfDaySeed();                                        // Seed random number geneator

    int dev = findCudaDevice( argc, (const char**)argv );   // Find GPU

    // Allocate host memory
    float* hXCS = new float[ SIZE ];
    float* hYCS = new float[ SIZE ];
    float*  hRS = new float[ SIZE ];
    float* hSum = new float[ SIZE / BLOCKSIZE ] ;

    // Fill random-value arrays
    for( int n = 0; n < SIZE; n++ )
    {
        hXCS[ n ] = Ranf( XCMIN, XCMAX );
        hYCS[ n ] = Ranf( YCMIN, YCMAX );
        hRS[  n ] = Ranf( RMIN,  RMAX  );
    }

    // Allocate device memory
    float *dXCS, *dYCS, *dRS, *dSum;

    dim3 dimsXCS( SIZE, 1, 1 );
    dim3 dimsYCS( SIZE, 1, 1 );
    dim3 dimsRS(  SIZE, 1, 1 );
    dim3 dimsSum( SIZE, 1, 1 );

    cudaError_t status;
    status = cudaMalloc( reinterpret_cast<void **>( &dXCS ), SIZE * sizeof( float ) );
        checkCudaErrors( status );
    status = cudaMalloc( reinterpret_cast<void **>( &dYCS ), SIZE * sizeof( float ) );
        checkCudaErrors( status );
    status = cudaMalloc( reinterpret_cast<void **>( &dRS  ), SIZE * sizeof( float ) );
        checkCudaErrors( status );
    status = cudaMalloc( reinterpret_cast<void **>( &dSum ), SIZE / BLOCKSIZE * sizeof( float ) );
        checkCudaErrors( status );

    // Copy memory to device:
    status = cudaMemcpy( dXCS, hXCS, SIZE * sizeof( float ), cudaMemcpyHostToDevice );
        checkCudaErrors( status );
    status = cudaMemcpy( dYCS, hYCS, SIZE * sizeof( float ), cudaMemcpyHostToDevice );
        checkCudaErrors( status );
    status = cudaMemcpy( dRS,  hRS,  SIZE * sizeof( float ), cudaMemcpyHostToDevice );
        checkCudaErrors( status );

    // Set up execution parameters
    dim3 threads( BLOCKSIZE, 1, 1 );
    dim3 grid( SIZE / threads.x, 1, 1 );

    // Create and start timer
    cudaDeviceSynchronize();

    // Allocate CUDA events for timing
    cudaEvent_t start, stop;
    status = cudaEventCreate( &start );
        checkCudaErrors( status );
    status = cudaEventCreate( &stop );
        checkCudaErrors( status );

    // Record start event
    status = cudaEventRecord( start, NULL );
        checkCudaErrors( status );

    // Execute kernel
    MonteCarlo <<< grid, threads >>>( dXCS, dYCS, dRS, dSum );

    // Record stop event
    status = cudaEventRecord( stop, NULL );
        checkCudaErrors( status );

    // Wait for stop event to complete
    status = cudaEventSynchronize( stop );
        checkCudaErrors( status );

    float msecTotal = 0.0f;
    status = cudaEventElapsedTime( &msecTotal, start, stop );
        checkCudaErrors( status );

    // Compute and print the performance
    double secondsTotal = 0.001 * (double)msecTotal;
    double multsPerSecond = (float)SIZE / secondsTotal;
    double megaMultsPerSecond = multsPerSecond / 1000000;
    printf( "Array Size = %10d, MegaCalculatiosn / Second = %10.2lf\n", SIZE, megaMultsPerSecond );

    // Copy result from device to host
    status = cudaMemcpy( hSum, dSum, (SIZE / BLOCKSIZE) * sizeof( float ), cudaMemcpyDeviceToHost );
        checkCudaErrors( status );

    // Check the probability:
    double sum = 0.;
    for( int i = 0; i < SIZE / BLOCKSIZE; i++ )
    {
        sum += (double)hSum[ i ];
    }
    double prob = sum / (double)SIZE;
    printf( "Probability = %10.2lf\n", prob );

    // Clean up memory
    delete[] hXCS;
    delete[] hYCS;
    delete[] hRS;
    delete[] hSum;

    status = cudaFree( dXCS );
        checkCudaErrors( status );
    status = cudaFree( dYCS );
        checkCudaErrors( status );
    status = cudaFree( dRS  );
        checkCudaErrors( status );
    status = cudaFree( dSum );
        checkCudaErrors( status );
    
    return 0;
}

float
Ranf( float low, float high )
{
    float r = (float) rand();           // 0 - RAND_MAX
    float t = r / (float) RAND_MAX;     // 0. - 1.

    return low + t * ( high - low );
}

int
Ranf( int ilow, int ihigh )
{
    float low = (float)ilow;
    float high = ceil( (float)ihigh );

    return (int) Ranf( low, high );
}

void
TimeOfDaySeed()
{
    struct tm y2k = { 0 };
    y2k.tm_hour = 0;
    y2k.tm_min  = 0;
    y2k.tm_sec  = 0;
    y2k.tm_year = 100;
    y2k.tm_mon  = 0;
    y2k.tm_mday = 1;

    time_t timer;
    time( &timer );
    double seconds = difftime( timer, mktime( &y2k ) );
    unsigned int seed = (unsigned int)( 1000. * seconds );  // milliseconds
    srand( seed );
}
