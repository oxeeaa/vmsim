

#include <stdint.h>
#include <stdlib.h>

#include "policy_fifo.h"
#include "policy.h"


bool vmem_policy_fifo_initialize(
    uint16_t* _,
    size_t mem_size,
    void** restrict ctx
) {
    vmem_policy_fifo_context_t* c = (vmem_policy_fifo_context_t*)malloc(sizeof(*c));
    if (!c) return false;

    c->last_used = (uint64_t*)calloc(mem_size, sizeof(*c->last_used));
    if (!c->last_used) {
        free(c);
        return false;
    }

    c->clock = 0;
    
    *ctx = (void*)c;

    return true;
}
bool vmem_policy_fifo_on_hit(
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

bool vmem_policy_fifo_on_miss(
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
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_fifo_context_t);
    //
    // Record when this frame has received a page and bump the logical clock
    //
    c->last_used[ppn] = c->clock++;

    return true;
}

bool vmem_policy_fifo_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn,
    void** restrict ctx
) {
    if (!VMEM_ASSERT_POLICY_CTX || !pout_frame || !pout_ppn)
        return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_fifo_context_t);
    
    size_t victim_ppn = 0;
    uint64_t oldest_time = c->last_used[0];

    //
    // Find the occupied frame with the smallest timestamp (first loaded)
    //
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


void vmem_policy_fifo_destroy(void** ctx) {
    if (!VMEM_ASSERT_POLICY_CTX) return;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_fifo_context_t);

    free(c->last_used);
    free(c);
}
