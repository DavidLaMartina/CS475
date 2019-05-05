arraymult: arraymult.cpp simd.p4.o
	g++ -DARRSIZE=${ARRSIZE} -o arraymult arraymult.cpp simd.p4.o -lm -fopenmp

simd.p4.o: simd.p4.h simd.p4.cpp
	g++ -c simd.p4.cpp -o simd.p4.o

clean:
	rm -rf simd.p4.o arraymult
