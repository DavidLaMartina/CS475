/* David LaMartina
 * lamartid@oregonstate.edu
 * Project 7B: Autocorrelation using CPU OpenMP, CPU SIMD, and GPU (CUDA)
 * CS475 Spr2019
 * Due June 10, 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#include "simd.p4.h"

#ifndef WRITENUMS
#define WRITENUMS   512
#endif

int     Size;
float*  Array;
float*  Sums;
FILE*   fp;

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
    Array = (float*)malloc( 2 * Size * sizeof( float ) );
    Sums  = (float*)malloc( 1 * Size * sizeof( float ) );

    // Scan in and duplicate the array to produce "wraparound" effect
    for( int i = 0; i < Size; i++ )
    {
        fscanf( fp, "%f", &Array[i] );
        Array[ i + Size ] = Array[i];
    }
    fclose( fp );
}

void writeData( )
{
    fp = fopen( "resultsSIMD.txt", "w" );
    if( fp == NULL )
    {
        fprintf( stderr, "Cannot open file 'resultsSIMD.txt' for writing\n" );
        exit( 1 );
    }
    for( int i = 0; i < WRITENUMS; i++ )
    {
        fprintf( fp, "%f\n", Sums[ i ] );
    }
    fclose( fp );
}

int main( int argc, char* argv[] )
{
    readData();

    double time0 = omp_get_wtime();

    for( int shift = 0; shift < Size; shift++ )
    {
        Sums[ shift ] = SimdMulSum( &Array[ 0 ], &Array[ 0 + shift ], Size );
    }

    double time1 = omp_get_wtime();
    double megaFuncPerSecond = (double)Size / ( time1 - time0 ) / 1000000.;
    printf( "%f megaFunctions per second\n", megaFuncPerSecond );

    writeData();
    free( Array );
    free( Sums  );

    return 0;
}
