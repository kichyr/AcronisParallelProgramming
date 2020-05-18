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
