/* David LaMartina
 * lamartid@oregonstate.edu
 * Project 3: Functional Decomposition
 * CS475 Spr2019
 * Due May 6, 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>

#define NUMTHREADS          4

// System 'state' globals
int     NowYear;            // 2019 - 2024
int     NowMonth;           // 0 - 11

float   Angle;              // Angle used for precip & temp updates by month
float   NowPrecip;          // inches of rain per month
float   NowTemp;            // temperature this month
float   NowHeight;          // grain height in inches
int     NowNumDeer;         // number of deer in current population

// Params defaults
// Grain growth in inches
// Temperature in degrees F
// Precipitation in inches
const float GRAIN_GROWS_PER_MONTH   =       8.0;
const float ONE_DEER_EATS_PER_MONTH =       0.5;

const float AVG_PRECIP_PER_MONTH    =       6.0;    // average
const float AMP_PRECIP_PER_MONTH    =       6.0;    // plus or minus
const float RANDOM_PRECIP           =       2.0;    // plus or minus noise

const float AVG_TEMP                =       50.0;   // average
const float AMP_TEMP                =       20.0;   // plus or minus
const float RANDOM_TEMP             =       10.0;   // plus or minus noise

const float MIDTEMP                 =       40.0;
const float MIDPRECRIP              =       10.0;

// Prototypes
void ParseConfig(std::string cFilePath);
void InitBarrier(int n);
void WaitBarrier();
void GrainDeer();
void Grain();
void Watcher();
void MyAgent();
void CalcAngle();
void CalcTemp();
void CalcPrecip();
void UpdateState();

// Thread-handling globals
omp_lock_t  Lock;
int         NumInThreadTeam;
int         NumAtBarrier;
int         NumGone;

int
main(int argc, char* argv[]){ 
    // Start date & time
    NowMonth    =   0;
    NowYear     =   2019;

    // Starting state
    NowNumDeer  =   1;
    NowHeight   =   1.;

    srand(time(0));
    omp_init_lock(&Lock);
    omp_set_num_threads(NUMTHREADS);    // same as # decomposed functions
    InitBarrier(NUMTHREADS);
    #pragma omp parallel sections
    {
        #pragma omp section
        { GrainDeer(); }
        #pragma omp section
        { Grain(); }
        #pragma omp section
        { Watcher(); }
        #pragma omp section
        { MyAgent(); }
    }

    return 0;
}

void ParseConfig(std::string cFilePath){
    // Config parse couresty of
    // https://walletfox.com/course/parseconfigfile.php
    std::ifstream cFile(cFilePath);
    if (cFile.is_open()){
        std::string line;
        while (getline(cFile, line)){
            line.erase(std::remove_if(line.begin(), line.end(), isspace),
                                      line.end());
            if (line[0] == '#' || line.empty()){
                continue;
            }
            int delimiterPos    = line.find("=");
            std::string name    = line.substr(0, delimiterPos);
            std::string value   = line.substr(delimiterPos + 1);
            int numVal          = std::stoi(value);

            std::cout << name << " " << numVal << '\n';
        }
    }else{
        std::cerr << "Couldn't open config file for reading.\n";
    }
}

void
InitBarrier(int n){
    NumInThreadTeam = n;
    NumAtBarrier = 0;
    omp_init_lock(&Lock);
}
void
WaitBarrier(){
    omp_set_lock(&Lock);
    {
        NumAtBarrier++;
        if(NumAtBarrier == NumInThreadTeam){    // release waiting threads
            NumGone = 0;
            NumAtBarrier = 0;
            
            // Let all other threads return before this one unlocks
            while( NumGone != NumInThreadTeam - 1);
            omp_unset_lock(&Lock);
            return;
        }
    }
    omp_unset_lock(&Lock);

    while (NumAtBarrier != 0);      // All threads wait here unti last arrives

    #pragma omp atomic              // and sets NumAtBarrier to 0
        NumGone++;
}

void GrainDeer(){

    return;
}

void Grain(){

    return;
}

void Watcher(){

    return;
}

void MyAgent(){

    return;
}

// State-updating functions
void CalcAngle(){
    Angle = ( 30. * (float)NowMonth + 15. ) * ( M_PI / 180. );
}

void CalcTemp(){
    float temp = AVG_TEMP - AMP_TEMP * cos(Angle);
    NowTemp = temp + (rand() % RANDOM_TEMP - RANDOM_TEMP + 1);
}

void CalcPrecip(){
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(Angle);
    NowPrecip = precip + (rand() % RANDOM_PRECIP - RANDOM_PRECIP + 1);
}

void UpdateState(){
    CalcAngle();
    CalcTemp();
    CalcPrecip();
}
