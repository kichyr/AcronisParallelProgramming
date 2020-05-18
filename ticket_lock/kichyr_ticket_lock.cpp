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
