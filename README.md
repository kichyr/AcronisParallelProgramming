## Ticket Lock

```
#include<thread>
#include<atomic>
#include<cassert>

// for most architectures cache line size equals to 64
#define CACHELINE_SIZE 64
#define assertm(exp, msg) assert(((void)msg, exp))

// KichyrTicketLock allows 
class lock {
private:
    // alignas(CACHELINE_SIZE) for preventing false sharing
    alignas(CACHELINE_SIZE) std::atomic_int now_serving;
    alignas(CACHELINE_SIZE) std::atomic_int next_ticket = {0};

    // Number of threads
    int number_of_threads;

public:
    lock(int num_thr): now_serving(0), next_ticket(0), number_of_threads(num_thr) {}
    ~lock() { assertm((next_ticket > number_of_threads), "Had happened some mistake!"); }

    // Try to acquire the spinlock
    inline void acquire() {
        const auto ticket = next_ticket.fetch_add(1, std::memory_order_relaxed);
        while (now_serving.load(std::memory_order_acquire) != ticket) {
            __asm volatile ("pause" ::: "memory");
        };        
    }

    // Try to release the spinlock
    void release() {
        const auto successor = now_serving.load(std::memory_order_acquire) + 1;
        now_serving.store(successor, std::memory_order_release);
    }
};

```
![Image description](https://github.com/kichyr/AcronisParallelProgramming/blob/master/bench_results/kichyr_ticket.png)

## TTAS Lock

```
#include<thread>
#include<atomic>
#include<cassert>

// for most architectures cache line size equals to 64
#define CACHELINE_SIZE 64
#define assertm(exp, msg) assert(((void)msg, exp))

// KichyrTicketLock allows 
class lock {
private:
    // alignas(CACHELINE_SIZE) for preventing false sharing
    alignas(CACHELINE_SIZE) std::atomic_bool Locked = {false};

    // Number of threads
    int number_of_threads;

public:
    lock(int num_thr): number_of_threads(num_thr) {}
    ~lock() {}

    // Try to acquire the spinlock
    inline void acquire() {
        do {
            //using WaitUntilLockIsFree function for preventing cache invalidation
            WaitUntilLockIsFree();
        } while (Locked.exchange(true, std::memory_order_acquire) == true);    
    }

    //
    inline void WaitUntilLockIsFree() const
    {
        while (Locked.load(std::memory_order_relaxed) == true);
    }

    // Try to release the spinlock
    inline void release()
    {
        Locked.store(false, std::memory_order_release);
    }
};

```


## Matrix
```
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
```
