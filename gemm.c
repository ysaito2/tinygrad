#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <immintrin.h>

#define N 64 

float A[N][N];
float B[N][N];
float C[N][N];
float val[N][N];

__m256 *Am = (__m256*)A;
__m256 *Bm = (__m256*)B; 
__m256 *Cm = (__m256*)C;

uint64_t nanos(){
	struct timespec start;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	return (uint64_t)start.tv_sec*1e9 + (uint64_t)start.tv_nsec;
}


#define BLOCK 8 
int main (){
	assert(N%BLOCK == 0);	

	FILE *f = fopen("/tmp/matmul", "rb");
	fread(A, 1, sizeof(float)*N*N, f);
	fread(B, 1, sizeof(float)*N*N, f);
	fread(val, 1, sizeof(float)*N*N, f);
	fclose(f); 

	// test
	// for (int k=0; k<N*N; k++){
	// 	A[k] = rand();
	// 	B[k] = rand(); 
	// }

	uint64_t start = nanos();
	for (int by = 0; by < N; by += BLOCK){
		for (int bx = 0; bx < N; bx += BLOCK){
		
			float tc[BLOCK][BLOCK];
			// compute
			for (int y = by; y < BLOCK; y++)	{
					for (int x = bx; x < BLOCK; x++){
						float acc = 0;
						for (int k =0; k<N; k++){
							acc += A[by+y][k] * B[k][bx+x];
						}

						tc[y][x] = acc;
					}
			}
			
			// store
			for (int y = 0; y < BLOCK; y++)	{
				for (int x = 0; x < BLOCK; x++){
					C[by+y][bx+x] = tc[y][x]; 
				}
			}
			// __m256 tc[BLOCK]; 

			// for (int y = 0; y < BLOCK; y++) {
			// 	__m256 tmp;

			// 	for (int k = 0; k < BLOCK; k+=8){
			// 		tmp = _mm256_fmadd_ps(
			// 			Am[((by+y)*N)/8 + k], 
			// 			Bm[(bx*N)/8 + k], tmp);
			// 	}

			// 	tc[y] = tmp; 
			// }

			// for (int y=0; y < BLOCK; y++){
			// 	Cm[((bx+y) *N)/8] = tc[y];
			// }
		}
	}

	uint64_t end = nanos();
	
	double gflop = (2.0*N*N*N)   * 1e-9;
	double s     = (end - start) * 1e-9;
	
	printf("%f GFLOP/S\n", gflop/s);

	for (int y = 0; y < N; y++){	
		for (int x = 0; x < N; x++)	{
			if (C[y][x] != val[y][x]){
				printf("Mismatch at %d x %d, %f != %f\n", y, x, C[y][x], val[y][x]);
				return(-1); 
			}
		}
	}

	return 0;
}