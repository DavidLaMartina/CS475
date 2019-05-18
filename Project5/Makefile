all:
	g++ -DNUM_ELEMENTS=${NUM_ELEMENTS} -DLOCAL_SIZE=${LOCAL_SIZE} -DCL_FILE=\"${CL_FILE}\" \
		-o arrayMult arrayMult.cpp \
		/scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp

clean:
	rm -rf arrayMult
