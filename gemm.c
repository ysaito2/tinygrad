#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>

#define N 64 

float A[N][N];
float B[N][N];
float C[N][N];

uint64_t nanos(){
	struct timespec start;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	return (uint64_t)start.tv_sec*1e9 + (uint64_t)start.tv_nsec;
}


#define BLOCK 32
int main (){
	assert(N%BLOCK == 0);	
	
	uint64_t start = nanos();
	for (int by = 0; by < N; by += BLOCK){
		for (int bx = 0; bx < N; bx += BLOCK){
			float tc[BLOCK][BLOCK];
			
			// // compute
			// for (int y = by; y < by+BLOCK; y++)	{
			// 		for (int x = bx; x < bx+BLOCK; x++){
			// 			float acc = 0;
			// 			for (int k =0; k<N; k++){
			// 				acc += A[y][k] * B[x][k];
			// 			}

			// 			tc[y][x] = acc;
			// 		}
			// }
			
			// store
			for (int y = 0; y < BLOCK; y++) {
				for (int x = 0; x < BLOCK; x++){
					C[by+y][bx+x] = tc[y][x];
				}
			}
		}
	}

	uint64_t end = nanos();
	
	double gflop = (2.0*N*N*N)   * 1e-9;
	double s     = (end - start) * 1e-9;
	
	printf("%f GFLOP/S\n", gflop/s);
	return 0;
}


