#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef     ARRSIZE
#define     ARRSIZE     1000
#endif

#ifndef     NUMTRIES
#define     NUMTRIES    10
#endif

int main(int argc, char* argv[]){
    printf("%d\n", ARRSIZE);

    return 0;
}
