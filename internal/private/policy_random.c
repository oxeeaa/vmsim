//
//
//      policy_random.c
//      Alan Ksenzov 2026-10-11
//
//


#include <errno.h>
#include <stdlib.h>
#include <time.h>

#include "policy_random.h"


bool vmem_policy_random_initialize(
    uint16_t* restrict trace, 
    size_t mem_size,
    void** restrict ctx
) {
    (void)trace;
    (void)mem_size;
    (void)ctx;

    srand((unsigned int)time(NULL));

    return true;
}

bool vmem_policy_random_on_hit(
    vmem_frame_t* restrict mem, 
    size_t mem_size,
    vmem_frame_t* restrict  frame,
    size_t                  ppn,
    void** restrict ctx
) {
    (void)mem;
    (void)mem_size;
    (void)ctx;
    (void)frame;
    (void)ppn;


    return true;
}

bool vmem_policy_random_on_miss(
    vmem_frame_t* restrict mem, 
    size_t                 mem_size,
    vmem_frame_t*          frame,
    size_t                 ppn, 
    void** restrict        ctx
) {
    (void)mem;
    (void)mem_size;
    (void)frame;
    (void)ppn;
    (void)ctx;

    return true;
}

bool vmem_policy_random_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn,
    void** restrict ctx
) {
    (void)ctx;

    if (
        mem == NULL || 
        mem_size == 0 ||
        pout_ppn == NULL ||
        pout_frame == NULL
    ) {
        errno = EINVAL;
        return false;
    }

    size_t ppn = (size_t)rand() % mem_size;

    *pout_frame = &mem[ppn];
    *pout_ppn   = ppn;

    return true;
}

void vmem_policy_random_destroy(void** ctx) {
    (void)ctx;
}
