#include <stdlib.h>
#include <cmath>
#include <omp.h>

#include "helpers.h"

// Randomization
float
Ranf(unsigned int* seedp, float low, float high){
    float r = (float)rand_r(seedp);
    return( low + r * (high - low) / (float)RAND_MAX );
}
int
Ranf(unsigned int* seedp, int ilow, int ihigh){
    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;
    return (int)(Ranf( seedp, low, high ));
}

// Precipitation and temperature calcs
float
Angle(int month){
    return (30. * (float)month + 15.) * (M_PI / 180.);
}

float
Temp(float avgTemp,
     float ampTemp,
     float randTemp,
     float angle,
     unsigned int* seedp)
{
    float temp = avgTemp - ampTemp * cos(angle);
    return temp + Ranf(seedp, -randTemp, randTemp);
}

float
Precip(float avgPPM,
       float ampPPM,
       float randPrecip,
       float angle,
       unsigned int* seedp)
{
    float precip = avgPPM + ampPPM * sin(angle);
    float nowPrecip = precip + Ranf(seedp, -randPrecip, randPrecip);
    if (nowPrecip < 0. )
        nowPrecip = 0;
    return nowPrecip;
}

// Factor calcs
float
SQR(float x){
    return x*x;
}

float
TempFactor(float nowTemp, float midTemp){
    return exp( -SQR( (nowTemp - midTemp) / 10. ) );
}

float
PrecipFactor(float nowPrecip, float midPrecip){
    return exp( -SQR( (nowPrecip - midPrecip) / 10. ) );
}

// Conversion
float
FtoC(float tempF){
    return (tempF - 32.) / 1.8;
}
