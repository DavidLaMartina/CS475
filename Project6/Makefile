# David LaMartina
# lamartid@oregonstate.edu
# Makefile for Project 6: Monte Carlo Simulation with CUDA
# CS475 Spr2019
# *** Makefile provided by CS475 / CS575 website ***

CUDA_PATH	=	/usr/local/apps/cuda/cuda-9.2
CUDA_BIN_PATH	=	$(CUDA_PATH)/bin
CUDA_NVCC	=	$(CUDA_BIN_PATH)/nvcc

arrayMul:	arrayMul.cu
		$(CUDA_NVCC) -o arrayMul  arrayMul.cu

proj6: proj6.cu
	$(CUDA_NVCC) -o proj6 proj6.cu

clean:
	rm -rf *.o* arrayMul proj6
