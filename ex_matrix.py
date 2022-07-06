#!/usr/bin/env python3 

import numpy as np
import time 

N = 4096

if __name__ == '__main__':
	A = np.random.randn(N,N).astype(np.float32)
	B = np.random.randn(N,N).astype(np.float32)

	
	start = time.monotonic()	
	C = A @ B
	end = time.monotonic()
	
	GFLOP = N*N*N*2 * 1e-9
	s = end - start 

	print(f"{GFLOP/s} GLOPS")

	with open("/tmp/matmul", "wb") as f:
		f.write(A)
		f.write(B)
		f.write(C)