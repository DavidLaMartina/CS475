#ifndef __HELPERS_H_INCLUDED__
#define __HELPERS_H_INCLUDED__

#include <stdlib.h>

// Randomization functions
float Ranf(unsigned int* seedp, float low, float high);
int   Ranf(unsigned int* seedp, int ilow, int ihigh);

// Precip & temperature calculations
float Angle(int month);

float Temp(float avgTemp,
           float ampTemp,
           float randTemp,
           float angle,
           unsigned int* seedp);

float Precip(float avgPPM,
             float ampPPM,
             float randPrecip,
             float angle,
             unsigned int* seedp);

// Factor calcs
float TempFactor(float nowTemp, float midTemp);
float PrecipFactor(float nowPrecip, float midPrecip);

// Conversion
float FtoC(float tempF);

#endif
