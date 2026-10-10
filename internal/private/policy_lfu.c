
#include <stdint.h>
#include <stdlib.h>

#include "policy.h"
#include "policy_lfu.h"


bool vmem_policy_lfu_initialize(
    uint16_t* restrict trace,
    size_t mem_size,
    void** restrict ctx
) {
    (void)trace;

    if (!ctx) return false;

    vmem_policy_lfu_context_t* c = malloc(sizeof(*c));
    if (!c) return false;

    c->access_count = calloc(mem_size, sizeof(*c->access_count));
    if (!c->access_count) {
        free(c);
        return false;
    }

    *ctx = (void*)c;

    return true;
}

bool vmem_policy_lfu_on_hit(
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
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_lfu_context_t);

    //
    // inectement access count 
    //
    c->access_count[ppn]++;

    return true;
}

bool vmem_policy_lfu_on_miss(
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
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_lfu_context_t);

    //
    // Initialize access count for this new page
    //
    c->access_count[ppn] = 1;

    return true;
}

bool vmem_policy_lfu_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn,
    void** restrict ctx
) {
    if (!VMEM_ASSERT_POLICY_CTX || !pout_ppn || !pout_frame)
        return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_lfu_context_t);

    size_t victim_ppn = 0;
    uint64_t fewest_accesses = c->access_count[0];

    //
    // Find a frame with smallest amount of accesess
    //
    for (size_t ppn = 1; ppn < mem_size; ++ppn) {
        if (c->access_count[ppn] < fewest_accesses) {
            fewest_accesses = c->access_count[ppn];
            victim_ppn = ppn;
        }
    }

    *pout_ppn   = victim_ppn;
    *pout_frame = &mem[victim_ppn];

    return true;
}

void vmem_policy_lfu_destroy(void** ctx) {
    if (!VMEM_ASSERT_POLICY_CTX) return;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_lfu_context_t);

    free(c->access_count);
    free(c);
}
