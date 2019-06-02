# David LaMartina
# lamartid@oregonstate.edu
# Project 7: Autocorrelation using CPU OpenMP, CPU SIMD, and GPU CUDA
# CS475 Spr2019
# Due June 10, 2019

CC				= g++
CFLAGS			= -lm -fopenmp

all: proj7OMP proj7SIMD proj7GPU

proj7OMP: proj7OMP.cpp
	$(CC) $(CFLAGS) -o proj7OMP -DNUMT=${NUMT} proj7OMP.cpp 

proj7SIMD: proj7SIMD.cpp
	$(CC) $(CFLAGS) -o proj7SIMD proj7SIMD.cpp simd.p4.cpp

proj7GPU: proj7GPU.cpp autoC.cl
	$(CC) $(CFLAGS) -o proj7GPU proj7GPU.cpp /scratch/cuda-7.0/lib64/libOpenCL.so

clean:
	rm -rf *.o* proj7OMP proj7SIMD proj7GPU
