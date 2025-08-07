#ifndef LLULU_LU_SYNC_H
#define LLULU_LU_SYNC_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
  #include <stdatomic.h>
  #define lu_sync_rbarrier() atomic_thread_fence(memory_order_acquire)
  #define lu_sync_wbarrier() atomic_thread_fence(memory_order_release)
  #define lu_sync_barrier() atomic_thread_fence(memory_order_seq_cst)
#elif defined(__x86_64__) || defined(__i386__)
  #define lu_sync_rbarrier() asm volatile("lfence" ::: "memory")
  #define lu_sync_wbarrier() asm volatile("sfence" ::: "memory")
  #define lu_sync_barrier() asm volatile("mfence" ::: "memory")
#elif defined(__GNUC__) || defined(__clang__)
  #define lu_sync_rbarrier() __sync_synchronize()
  #define lu_sync_wbarrier() __sync_synchronize()
  #define lu_sync_barrier() __sync_synchronize()
#elif defined(_WIN32)
  #include <windows.h>
  #define lu_sync_rbarrier() MemoryBarrier()
  #define lu_sync_wbarrier() MemoryBarrier()
  #define lu_sync_barrier() MemoryBarrier()
#else
  #define lu_sync_rbarrier() asm volatile("" ::: "memory")
  #define lu_sync_wbarrier() asm volatile("" ::: "memory")
  #define lu_sync_barrier() asm volatile("" ::: "memory")
#endif

#endif /* LLULU_LU_SYNC_H */
