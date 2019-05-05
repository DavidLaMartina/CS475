/* David LaMartina
 * lamartid@oregonstate.edu
 * CS475 Spr2019
 * Project4: Vectorized Array Multiplication and Reduction using SSE
 * Due May 13, 2019
 */

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "simd.p4.h"    // Contains actual calculation functions being tested

#ifndef     ARRSIZE
#define     ARRSIZE     1000
#endif

#ifndef     NUMTRIES
#define     NUMTRIES    10
#endif

int main(int argc, char* argv[]){
    // First output array size
    printf("%d\t", ARRSIZE);

    // Setup: arrays and maxPerformance
    float *a = new float[ARRSIZE];
    float *b = new float[ARRSIZE];
    float *c = new float[ARRSIZE];
    double maxPerformanceSSE;
    double maxPerformanceNonSSE;

    // SIMD Array Multiplication
    maxPerformanceSSE = 0;
    for (int t = 0; t < NUMTRIES; t++){
        double time0 = omp_get_wtime();

        SimdMul(a, b, c, ARRSIZE);
        
        double totalTime = omp_get_wtime() - time0;
        double megaFuncPerSecond = (double)ARRSIZE / totalTime / 1000000;
        
        if (megaFuncPerSecond > maxPerformanceSSE){
            maxPerformanceSSE = megaFuncPerSecond;
        }
    }
    // printf("%.3lf\t", maxPerformanceSSE);

    // Non-SIMD Array Multiplication
    maxPerformanceNonSSE = 0;
    for (int t = 0; t < NUMTRIES; t++){
        double time0 = omp_get_wtime();

        NonSimdMul(a, b, c, ARRSIZE);
        
        double totalTime = omp_get_wtime() - time0;
        double megaFuncPerSecond = (double)ARRSIZE / totalTime / 1000000;

        if (megaFuncPerSecond > maxPerformanceNonSSE){
            maxPerformanceNonSSE = megaFuncPerSecond;
        }
    }
    // printf("%.3lf\t", maxPerformanceNonSSE);

    // Output speedup for arr. mult - SSE over non-SSE
    printf("%.3lf\t", (maxPerformanceSSE / maxPerformanceNonSSE));

    // SIMD Array Reduction
    maxPerformanceSSE = 0;
    for (int t = 0; t < NUMTRIES; t++){
        double time0 = omp_get_wtime();

        SimdMulSum(a, b, ARRSIZE);
        
        double totalTime = omp_get_wtime() - time0;
        double megaFuncPerSecond = (double)ARRSIZE / totalTime / 1000000;

        if (megaFuncPerSecond > maxPerformanceSSE){
            maxPerformanceSSE = megaFuncPerSecond;
        }
    }
    // printf("%.3lf\t", maxPerformanceSSE);

    // Non-SIMD Array Reduction
    maxPerformanceNonSSE = 0;
    for (int t = 0; t < NUMTRIES; t++){
        double time0 = omp_get_wtime();

        NonSimdMulSum(a, b, ARRSIZE);
        
        double totalTime = omp_get_wtime() - time0;
        double megaFuncPerSecond = (double)ARRSIZE / totalTime / 1000000;

        if (megaFuncPerSecond > maxPerformanceNonSSE){
            maxPerformanceNonSSE = megaFuncPerSecond;
        }
    }
    // printf("%.3lf\t", maxPerformanceNonSSE);

    // Output speedup for arr. reduce - SSE over non-SSE
    printf("%.3lf\n", (maxPerformanceSSE / maxPerformanceNonSSE));
    
    // Free mem & exit
    delete[] a;
    delete[] b;
    delete[] c;
    
    return 0;
}
