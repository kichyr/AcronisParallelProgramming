#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define M(mA, x, y) (mA[(x) * N + (y)])
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef struct multiply_worker_args {
    int M;
    int N;
    int K;
    const double * A;
    const double * B;
    double * C;
} multiply_worker_args;

void printMatrix(int N, int M, const double * A) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
            printf("%f ", M(A, i, j));
        printf("\n");
    }
    printf("---------\n");
}

void multiplyWorker(void* args) {
    struct multiply_worker_args * m_args = (struct multiply_worker_args *)args;
    for (int i = 0; i < m_args->N; ++i) {
        double * c = m_args->C + i * m_args->M;
        for (int j = 0; j < m_args->M; ++j)
            c[j] = 0;
        for (int k = 0; k < m_args->K; ++k)
        {
            const double * b = m_args->B + k * m_args->K;
            double a = m_args->A[i*m_args-> K + k];
            for (int j = 0; j < m_args->M; ++j)
                c[j] += a * b[j];
        }
    }
}

void multiply(int TN, int M, int N, int K, const double * A, const double * B, double * C) {
    TN = MIN(TN, N);
    int ThreadNumber = (N % TN != 0) ? TN + 1 : TN;
    ThreadNumber = MAX(ThreadNumber, 0);
    multiply_worker_args * args = (multiply_worker_args *)malloc(ThreadNumber * sizeof(multiply_worker_args));
    pthread_t* threads = (pthread_t *)malloc(ThreadNumber * sizeof(pthread_t));
    int prev_row = 0, next_row = 0;
    int used_processes = 0;
    for (int i = 0; next_row != N; i++, used_processes++) {
        next_row = MIN(prev_row + N / TN, N);
        args[i].M = M;
        args[i].K = K;
        args[i].B = B;
        args[i].N = next_row - prev_row;
        args[i].A = A + prev_row * K;
        args[i].C = C + prev_row * K; 
        int err = pthread_create(&threads[i], NULL, (void *)&multiplyWorker, &args[i]);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        prev_row = next_row;
    }
    
    for (int i = 0; i < used_processes; i++) {
        pthread_join(threads[i], NULL);
    }
    free(threads);
    free(args);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("not enoughs args");
		return 1;
	}
    // row number
    int N = atoi(argv[1]);
    int ThreadNumber = atoi(argv[2]);
    double* A = (double*)malloc(N * N * sizeof(A[0]));
	double* B = (double*)malloc(N * N * sizeof(B[0]));
	double* C = (double*)malloc(N * N * sizeof(C[0]));
    
    for (int i = 0; i < (N * N); i++)
		A[i] = rand();

	for (int i = 0; i < (N * N); i++)
		B[i] = rand();

	for (int i = 0; i < (N * N); i++)
		C[i] = i;

    multiply(ThreadNumber, N, N, N, A, B, C);
    free(A);
    free(B);
    free(C);
}