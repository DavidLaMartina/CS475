#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
#ifndef _OPENMP
    fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
    return 1;
#endif

    double time0 = omp_get_wtime();

    int i;
    for (i = 0; i < 1000000; i++)
        ;

    double time1 = omp_get_wtime();

    fprintf(stderr, "Elapsed time = %10.2lf microseconds\n", 1000000. *(time1 - time0));

    return 0;
}
