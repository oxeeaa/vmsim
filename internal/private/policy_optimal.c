//
//
//      policy_optimal.c
//      Alan Ksenzov 2026-10-11
//
//



#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "array.h"
#include "policy_optimal.h"
#include "memory.h"
#include "policy.h"


bool vmem_policy_optimal_initialize(
    uint16_t* restrict trace,
    size_t             mem_size,
    void**    restrict ctx
) {
    (void)mem_size;
    
    vmem_policy_optimal_context_t* c = (vmem_policy_optimal_context_t*)malloc(sizeof(*c));
    if (!c)
        return false;
    
    c->trace = trace;
    c->trace_size = ARRAY_SIZE(trace);
    c->current_idx = 0;

    *ctx = (void*)(c);

    return true;
}

bool vmem_policy_optimal_on_hit(
    vmem_frame_t* restrict mem, 
    size_t                 mem_size,
    vmem_frame_t* restrict frame,
    size_t                 ppn, 
    void**        restrict ctx
) {
    //
    // Advance the timeline
    //
    (void)mem;
    (void)mem_size;
    (void)frame;
    (void)ppn;

    if (!VMEM_ASSERT_POLICY_CTX) return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_optimal_context_t);

    ++c->current_idx;

    return true;
}

bool vmem_policy_optimal_on_miss(
    vmem_frame_t* restrict mem, 
    size_t                 mem_size, 
    vmem_frame_t* restrict frame,
    size_t                 ppn,
    void**        restrict ctx
) {
    //
    // Advance the timeline
    //
    (void)mem;
    (void)mem_size;
    (void)frame;
    (void)ppn;

    if (!VMEM_ASSERT_POLICY_CTX) return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_optimal_context_t);
    
    ++c->current_idx;

    return true;
}

bool vmem_policy_optimal_select_victim(
    vmem_frame_t*  restrict mem, 
    size_t                  mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t*        restrict pout_ppn,
    void**         restrict ctx
) {
    (void)mem;
    (void)mem_size;
    (void)pout_frame;
    (void)pout_ppn;
    (void)ctx;
    
    if (!VMEM_ASSERT_POLICY_CTX) return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_optimal_context_t);
    
    //
    // Find a frame that will not be used for the longest time
    //

    size_t victim_ppn = 0;
    size_t furthest_next_use = 0;
    //
    // Check all frames and scan forwards in the trace
    //
    for (size_t ppn = 0; ppn < mem_size; ++ppn) {
        size_t frame_vpn = mem[ppn].vpn;
        size_t next_use = SIZE_MAX;

        //
        // Scan forward starting from next access
        //
        for (size_t k = c->current_idx + 1; k < c->trace_size; ++k) {
            if ((c->trace[k] / VMEM_PAGE_SZ) == frame_vpn) {
                next_use = k;
                break;
            }
        }

        //
        // If page is never used again; it is the ideal victim
        //
        if (next_use == SIZE_MAX) {
            victim_ppn = ppn;
            break;
        }

        //
        // Track frame whose next use is furthest in the future
        //
        if (next_use > furthest_next_use) {
            furthest_next_use = next_use;
            victim_ppn = ppn;
        }
    }

    *pout_ppn   = victim_ppn;
    *pout_frame = &mem[victim_ppn];

    return true;
}

void vmem_policy_optimal_destroy(void** ctx) {
    if (!VMEM_ASSERT_POLICY_CTX) return;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_optimal_context_t);
    free(c);
}

