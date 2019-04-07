/* David LaMartina
 * lamartid@oregonstate.edu
 * CS475e Spr2019
 * Project #0 - Simple OpenMP Experiment
 *
 * Test speed using 1 thread vs. 4 threads on pairwise multiplication of
 * floating point arrays
 */

#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <float.h>

using std::rand;
using std::srand;

#ifndef NUMTHREADS
#define NUMTHREADS      4
#endif
#ifndef ARRAYSIZE
#define ARRAYSIZE       1000000
#endif
#ifndef NUMTRIES
#define NUMTRIES        100
#endif

float A[ARRAYSIZE];
float B[ARRAYSIZE];
float C[ARRAYSIZE];

int main(int argc, char* argv[]){

#ifndef _OPENMP
    fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
    return 1;
#endif
    omp_set_num_threads( NUMTHREADS );
    printf("Using %d threads on a system with %d cores\n",
            NUMTHREADS, omp_get_num_procs());

    double maxMegaMults = 0;
    double minExTime = FLT_MAX;

    int t;
    for(t = 0; t < NUMTRIES; t++){
        // Assigning vars each time...seems to make slight difference (negative)
        // in performance of actual parallel code
        for(int i = 0; i < ARRAYSIZE; i++){
            A[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            B[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }
        double time0 = omp_get_wtime();

        #pragma omp parallel for
        for(int i = 0; i < ARRAYSIZE; i++){
            C[i] = A[i] * B[i];
        }
        double time1 = omp_get_wtime();
        double exTime = time1 - time0;
        if (exTime < minExTime)
            minExTime = exTime;
        double megaMults = (double)ARRAYSIZE / exTime / 1000000.;
        if(megaMults > maxMegaMults)
            maxMegaMults = megaMults;
    }
    printf("Peak performance with %d threads = %8.2lf MegaMults / sec (run %d)\n",
            NUMTHREADS, maxMegaMults, t);
    printf("Minimum array multiplication execution time: %.4lf seconds (run %d)\n",
            minExTime, t);

    return 0;
}
