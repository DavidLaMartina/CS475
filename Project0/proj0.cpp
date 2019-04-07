#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#ifndef NUMTHREADS
#define NUMTHREADS      4
#endif
#define ARRAYSIZE       10000000
#define NUMTRIES        100

float A[ARRAYSIZE];
float B[ARRAYSIZE];
float C[ARRAYSIZE];

int main(int argc, char *argv[]){
#ifndef _OPENMP
    fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
    return 1;
#endif

    omp_set_num_threads(NUMTHREADS);

    double minExTime = FLT_MAX;
    double maxMegaMults = 0.;

    for(int t = 0; t < NUMTRIES; t++){
        double time0 = omp_get_wtime();

        #pragma omp parallel for
        for(int i = 0; i < ARRAYSIZE; i++)
            C[i] = A[i] * B[i];

        double time1 = omp_get_wtime();
        double exTime = time1 - time0;
        if(exTime < minExTime)
            minExTime = exTime;
        double megaMults = (double)ARRAYSIZE / exTime / 1000000;
        if(megaMults > maxMegaMults)
            maxMegaMults = megaMults;
    }
    // Output min time and max perf as "fields" to be piped into another prog
    printf("%8.2lf\t%.4lf\n", maxMegaMults, minExTime);

    return 0;
}
