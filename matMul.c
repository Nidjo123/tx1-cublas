#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// gcc -o matMul -O3 -msse4 matMul.c

#define MINN 2
#define MAXN 1024
#define ITERS 10
#define ALIGNMENT 16

typedef int Type;

Type sum(Type *mat, int n) {
	int i;
	Type res = 0;
	
	for (i = 0; i < n * n; i++) res += mat[i];
	
	return res;
}

void mult(Type *A, Type *B, Type *C, int n) {
	int i, j, k;
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			C[i * n + j] = 0;
			for (k = 0; k < n; k++) {
				C[i * n + j] += A[i * n + k] * B[k * n + j];
			}
		}
	}
}

int main(void) {
	int i, j, n, iter;
	Type *matA, *matB, *matC;
	clock_t start;
	double elapsed = 0.0;

	if (posix_memalign((void**)&matA, ALIGNMENT, MAXN * MAXN * sizeof(Type))) {
	  printf("Cannot allocate memory!\n");
	  return -1;
	}
	
        if (posix_memalign((void**)&matB, ALIGNMENT, MAXN * MAXN * sizeof(Type))) {
	  printf("Cannot allocate memory!\n");
	  free(matA);
	  return -1;
	}
	
	if (posix_memalign((void**)&matC, ALIGNMENT, MAXN * MAXN * sizeof(Type))) {
	  printf("Cannot allocate memory!\n");
	  free(matA);
	  free(matB);
	  return -1;
	}
	
	for (n = MINN; n <= 1024; n *= 2) {
		for (i = 0; i < n * n; i++) 
			matA[i] = matB[i] = matC[i] = 0;
		
		start = clock();
		for (iter = 0; iter < ITERS; iter++) {
			mult(matA, matB, matC, n);
		}
		elapsed = clock() - start;
		
		printf("n = %d, prosjecno vrijeme po mnozenju: %g sekundi\n", n, elapsed / ITERS / CLOCKS_PER_SEC);
	}
	
	printf("Suma: %d\n", sum(matC, n / 2));
	
	free(matA);
	free(matB);
	free(matC);
	
	return 0;
}
