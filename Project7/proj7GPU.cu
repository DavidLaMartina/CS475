/* David LaMartina
 * lamartid@oregonstate.edu
 * Project 7B: Autocorrelation using CPU OpenMP, CPU SIMD, and GPU (CUDA)
 * CS475 Spr2019
 * Due June 10, 2019
 */

#include <stdio.h>
#include <stdlib.h>

int     Size;
float*  Array;
float*  Sums;
FILE*   fp;
int     i;

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
    for( i = 0; i < Size; i++ )
    {
        fscanf( fp, "%f", &Array[i] );
        Array[ i + Size ] = Array[i];
    }
    fclose( fp );
}

int main( int argc, char* argv[] )
{


    return 0;
}
