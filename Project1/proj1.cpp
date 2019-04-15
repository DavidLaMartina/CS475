#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// Define number of threads:
#ifndef NUMT
#define NUMT            1
#endif
// number of trials for Monte Carlo simulation
#ifndef NUMTRIALS
#define NUMTRIALS       1000000
#endif
// number of tries to discover max performance
#ifndef NUMTRIES
#define NUMTRIES        10
#endif

// Ranges for random numbers:
const float XCMIN =     -1.0;
const float XCMAX =      1.0;
const float YCMIN =      0.0;
const float YCMAX =      2.0;
const float RMIN  =      0.5;
const float RMAX  =      2.0;

// Function prototypes
float           Ranf(float, float);
int             Ranf(int, int);
void            TimeOfDaySeed();

// Main program
int main(int argc, char* argv[]){
#ifndef _OPENMP
    fprintf(stderr, "No OpenMP support!\n");
    return 1;
#endif

    TimeOfDaySeed();                    // Seed random number generator
    omp_set_num_threads(NUMT);          // set number of threads to use in for loop

    // Set up arrays outside pragma / timing
    float *xcs = new float[NUMTRIALS];
    float *ycs = new float[NUMTRIALS];
    float *rs  = new float[NUMTRIALS];

    for(int n = 0; n < NUMTRIALS; n++){
        xcs[n] = Ranf(XCMIN, XCMAX);
        ycs[n] = Ranf(YCMIN, YCMAX);
        rs[n]  = Ranf(RMIN, RMAX);
    }
    // Get ready to recdord max performance and proability:
    float maxPerformance = 0;
    float currentProb    = 0;

    // Looking for maximum performance out of all tries
    for(int t = 0; t < NUMTRIES; t++){
        double time0 = omp_get_wtime();

        int numHits = 0;
        #pragma omp parallel for default(none) shared(xcs, ycs, rs) reduction(+:numHits)
        for(int n = 0; n < NUMTRIALS; n++){
            // randomize location and radius of circle
            float xc = xcs[n];
            float yc = ycs[n];
            float  r =  rs[n];

            // solve for intersection using quadratic formula
            float a = 2;
            float b = -2. * (xc + yc);
            float c = xc*xc + yc*yc - r*r;
            float d = b*b - 4.*a*c;

            // If d < 0, circle was missed
            if(d < 0) continue;

            // Else we've hit the circle in some way - determine how
            d = sqrt(d);
            float t1 = (-b + d) / (2. * a);
            float t2 = (-b - d) / (2. * a);
            float tmin = t1 < t2 ? t1 : t2;     // Only care about first intersection

            // If tmin < 0, circle engulfs laser pointer
            if(tmin < 0) continue;

            // WHERE does it intersect?
            float xcir = tmin;
            float ycir = tmin;
            // Get unitized normal vector @ point of intersection
            float nx = xcir - xc;
            float ny = ycir - yc;
            float n  = sqrt(nx*nx + ny*ny);
            nx /= n;        // unit vector
            ny /= n;        // unit vector
            // Get unitized incoming vector:
            float inx = xcir - 0.;
            float iny = ycir - 0.;
            float in  = sqrt(inx*inx + iny*iny);
            inx /= in;      // unit vector
            iny /= in;      // unit vector
            // get outgoing (bounced) vector
            float dot = inx*nx + iny*ny;
            float outx = inx - 2.*nx*dot;   // angle of reflection=angle of incidence
            float outy = iny - 2.*ny*dot;   // angle of reflection=angle of incidence
            // find out if it hits plate:
            float t = (0. - ycir) / outy;

            // If t < 0, reflected beam went up instead of down
            if(t < 0) continue;

            // otherwise, beam hit plate...increment number of hits, continue
            numHits++;
        }
        double time1 = omp_get_wtime();
        double megaTrialsPerSecond = (double)NUMTRIALS / (time1 - time0) / 1000000;
        if(megaTrialsPerSecond > maxPerformance)
            maxPerformance = megaTrialsPerSecond;
        currentProb = (float)numHits/(float)NUMTRIALS;
    }
    // Output tab-separated #threads, #trials, probability, and performance
    printf("%d\t%d\t%.4lf\t%8.2lf\n", NUMT, NUMTRIALS, currentProb, maxPerformance);
}


// Helpers
#include <stdlib.h>

float
Ranf(float low, float high){
    float r = (float)rand();            // 0 - RAND_MAX
    float t = r / (float)RAND_MAX;      // 0. - 1.
    return low + t * (high - low);
}
int
Ranf(int ilow, int ihigh){
    float low = (float)ilow;
    float high = ceil((float)high);
    return (int)Ranf(low, high);
}
void
TimeOfDaySeed(){
    struct tm y2k = {0};
    y2k.tm_hour = 0;    y2k.tm_min = 0; y2k.tm_sec = 0;
    y2k.tm_year = 100;  y2k.tm_mon = 0; y2k.tm_mday = 1;

    time_t timer;
    time(&timer);
    double seconds = difftime(timer, mktime(&y2k));
    unsigned int seed = (unsigned int)(1000. * seconds);    // milliseconds
    srand(seed);
}
