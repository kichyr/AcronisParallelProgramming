#include "lock.h"
#include "kichyr_ttas_lock.cpp"
#include <string>
#include <atomic>
#include <iostream>
#include <unistd.h>
#include <thread> 
#include <cstdlib>
#include <cassert>

/*
 * Allocates resources for the lock.
 *
 * n_threads is number of threads that will
 * share this lock. lock_alloc() may ignore
 * this argument.
 *
 * It is guaranteed to be called only once from a
 * calling program, so allocation in static memory is OK.
 *
 * Returns pointer to the lock in case
 * of success and (void*)(NULL) otherwise.
 */
lock_t* lock_alloc(long unsigned n_threads) {
    lock_t *ticket_lock = new lock_t(n_threads);
    if(ticket_lock == NULL) {
        return NULL;
    }
    return ticket_lock;
}

/*
 * Acquires the lock.
 *
 * 'arg' is the value returned by lock_alloc() function.
 *
 * Returns zero in case of success and nonzero value
 * otherwise, e.g. if lock is in inconsistent state.
 */
int lock_acquire(lock_t* arg) {
    if(arg == NULL) {
        return EXIT_FAILURE;
    }
    arg->acquire();
    return EXIT_SUCCESS;
}

/*
 * Releases the lock.
 *
 * 'arg' is the value returned by lock_alloc() function.
 *
 * Returns zero in case of success and nonzero value
 * otherwise, e.g. if lock was not acquired by the caller.
 */
int lock_release(lock_t* arg) {
    if(arg == NULL) {
        return EXIT_FAILURE;
    }
    arg->release();
    return EXIT_SUCCESS;
}

/*
 * Frees resources associated with the lock.
 *
 * 'arg' is the value returned by lock_alloc() function.
 *
 * It is guaranteed to be called only once for the
 * successfully alloc()-ed lock.
 *
 * Returns zero in case of success and nonzero value
 * otherwise, e.g. if lock was not released beforehand.
 */
int lock_free(lock_t* arg) {
    delete arg;
    return EXIT_SUCCESS;
}