#!/usr/bin/env python3 

import numpy as np
import time 

N = 4096

if __name__ == '__main__':
	A = np.random.randn(N,N).astype(np.float32)
	B = np.random.randn(N,N).astype(np.float32)

	while True: 	
		start = time.monotonic()	
		C = A @ B
		end = time.monotonic()
		
		GFLOP = N*N*N*2 * 1e-9
		s = end - start 

		print(f"{GFLOP/s} GLOPS")