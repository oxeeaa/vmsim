//
//
//      policy_mru.c
//      Alan Ksenzov 2026-10-11
//
//


#include <stdint.h>
#include <stdlib.h>

#include "policy.h"
#include "policy_mru.h"


bool vmem_policy_mru_initialize(
    uint16_t* restrict trace,
    size_t mem_size,
    void** restrict ctx
) {
    (void)trace;

    if (!ctx) return false;

    vmem_policy_mru_context_t* c = malloc(sizeof(*c));
    if (!c) return false;

    c->last_used = calloc(mem_size, sizeof(*c->last_used));
    if (!c->last_used) {
        free(c);
        return false;
    }

    c->clock = 0;

    *ctx = (void*)c;

    return true;
}

bool vmem_policy_mru_on_hit(
    vmem_frame_t* restrict mem, 
    size_t mem_size,
    vmem_frame_t* restrict  frame,
    size_t                  ppn, 
    void** restrict ctx
) {
    (void)mem;
    (void)mem_size;
    (void)frame;

    if (!VMEM_ASSERT_POLICY_CTX) return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_mru_context_t);
    
    //
    // Update timesptamp with current clock tick
    //
    c->last_used[ppn] = c->clock++;

    return true;
}

bool vmem_policy_mru_on_miss(
    vmem_frame_t* restrict mem, 
    size_t                 mem_size, 
    vmem_frame_t*          frame,
    size_t                 ppn,
    void** restrict ctx
) {
    (void)mem;
    (void)mem_size;
    (void)frame;

    if (!VMEM_ASSERT_POLICY_CTX) return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_mru_context_t);
    
    //
    // Record initial load timestamp
    //
    c->last_used[ppn] = c->clock++;

    return true;
}

bool vmem_policy_mru_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn,
    void** restrict ctx
) {
    if (!VMEM_ASSERT_POLICY_CTX || !pout_ppn || !pout_frame)
        return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_mru_context_t);

    size_t victim_ppn = 0;
    uint64_t newest_frame = c->last_used[0];

    //
    // Find most recently used frame; i.e. frame that has largest clockl
    //
    for (size_t ppn = 1; ppn < mem_size; ++ppn) {
        if (c->last_used[ppn] > newest_frame) {
            newest_frame = c->last_used[ppn];
            victim_ppn = ppn;
        }
    }

    *pout_ppn   = victim_ppn;
    *pout_frame = &mem[victim_ppn];

    return true;
}

void vmem_policy_mru_destroy(void** ctx) {
    if (!VMEM_ASSERT_POLICY_CTX) return;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_mru_context_t);

    free(c->last_used);
    free(c);

    *ctx = NULL;
}
