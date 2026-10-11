//
//
//      policy_lru.c
//      Alan Ksenzov 2026-10-11
//
//


#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "policy_lru.h"
#include "policy.h"


bool vmem_policy_lru_initialize(
    uint16_t* restrict trace, 
    size_t mem_size,
    void** restrict ctx
) {
    (void)trace;

    vmem_policy_lru_context_t* c = (vmem_policy_lru_context_t*)malloc(sizeof(*c));
    if (!c) return false;

    c->last_used = (uint64_t*)calloc(mem_size, sizeof(*c->last_used));
    if (!c->last_used) {
        free (c);
        return false;
    }

    c->clock = 0;

    *ctx = (void*)c;

    return true;
}

bool vmem_policy_lru_on_hit(
    vmem_frame_t* restrict mem, 
    size_t                 mem_size,
    vmem_frame_t*          frame,
    size_t                 ppn,
    void** restrict        ctx
) {
    //
    // Bump up the clock on hit. Lowest clock gets evicted (least recently used)
    //
    (void)mem;
    (void)mem_size;
    (void)frame;
    
    if (!VMEM_ASSERT_POLICY_CTX) return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_lru_context_t);
    
    c->last_used[ppn] = c->clock++;

    return true;
}

bool vmem_policy_lru_on_miss(
    vmem_frame_t* restrict mem, 
    size_t                 mem_size,
    vmem_frame_t*          frame,
    size_t                 ppn, 
    void** restrict        ctx
) {
    //
    // Record access time when frame received a page
    //
    (void)mem;
    (void)mem_size;
    (void)frame;
    
    if (!VMEM_ASSERT_POLICY_CTX) return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_lru_context_t);
    
    c->last_used[ppn] = c->clock++;

    return true;
}

bool vmem_policy_lru_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn,
    void** restrict ctx
) {
    //
    // Evict least recently used page (lowest clock)
    //
    if (!VMEM_ASSERT_POLICY_CTX || !pout_frame || !pout_ppn) return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_lru_context_t);

    size_t victim_ppn = 0;
    uint64_t oldest_time = c->last_used[0];
    for (size_t ppn = 1; ppn < mem_size; ++ppn) {
        if (c->last_used[ppn] < oldest_time) {
            oldest_time = c->last_used[ppn];
            victim_ppn = ppn;
        }
    }

    *pout_ppn = victim_ppn;
    *pout_frame = &mem[victim_ppn];

    return true;
}

void vmem_policy_lru_destroy(void** ctx) {
    if (!ctx || !*ctx) return;

    vmem_policy_lru_context_t* c = (vmem_policy_lru_context_t*)(*ctx);
    
    free(c->last_used);
    free(c);

    *ctx = NULL;
}
