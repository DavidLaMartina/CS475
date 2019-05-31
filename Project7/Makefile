# David LaMartina
# lamartid@oregonstate.edu
# Project 7: Autocorrelation using CPU OpenMP, CPU SIMD, and GPU CUDA
# CS475 Spr2019
# Due June 10, 2019

CC 		= g++
CFLAGS 	= -lm

all: proj7OMP proj7SIMD proj7GPU

proj7OMP: proj7OMP.cpp
	$(CC) $(CFLAGS) -o proj7OMP -DNUMT=${NUMT} proj7OMP.cpp -fopenmp

proj7SIMD: proj7SIMD.cpp
	$(CC) $(CFLAGS) -o proj7SIMD proj7SIMD.cpp simd.p4.cpp -fopenmp

proj7GPU:

clean:
	rm -rf *.o* proj7OMP proj7SIMD proj7GPU
