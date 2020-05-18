#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define M(mA, x, y) (mA[(x) * N + (y)])

void multiply(int M, int N, int K, const double * A, const double * B, double * C)
{
    for (int i = 0; i < M; ++i)
    {
        double * c = C + i * N;
        for (int j = 0; j < N; ++j)
            c[j] = 0;
        for (int k = 0; k < K; ++k)
        {
            const double * b = B + k * N;
            double a = A[i*K + k];
            for (int j = 0; j < N; ++j)
                c[j] += a * b[j];
        }
    }
}

void printMatrix(int N, int M, const double * A) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
            printf("%f ", M(A, i, j));
        printf("\n");
    }
    printf("---------\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
		fprintf(stderr, "Usage: ./main <square_matrix_size>\n");
		return 1;
	}
    // row number
    int N = atoi(argv[1]);
    double* A = (double*)malloc(N * N * sizeof(A[0]));
	double* B = (double*)malloc(N * N * sizeof(B[0]));
	double* C = (double*)malloc(N * N * sizeof(C[0]));
    
    for (int i = 0; i < (N * N); i++)
		A[i] = rand();

	for (int i = 0; i < (N * N); i++)
		B[i] = rand();

	for (int i = 0; i < (N * N); i++)
		C[i] = 0;

    multiply(N, N, N, A, B, C);
}