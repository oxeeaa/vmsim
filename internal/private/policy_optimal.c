

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "array.h"
#include "policy_optimal.h"
#include "memory.h"
#include "policy.h"


bool vmem_policy_optimal_initialize(
    uint16_t* restrict trace,
    size_t mem_size,
    void** restrict ctx
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
    size_t mem_size,
    vmem_frame_t* restrict  frame,
    size_t                  ppn, 
    void** restrict ctx
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
    vmem_frame_t*          frame,
    size_t                 ppn,
    void** restrict ctx
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
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn,
    void** restrict ctx
) {
    (void)mem;
    (void)mem_size;
    (void)pout_frame;
    (void)pout_ppn;
    (void)ctx;
    
    if (!VMEM_ASSERT_POLICY_CTX) return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_optimal_context_t);
    // [current_trace_idx] [future]
    // i need to predict the future
    //
    
    // I look at memory acceses that will happen in future
    // I check if RESIDENT
    //
    // if not resident? swap?

    for (size_t i = c->current_idx + 1; i < c->trace_size; ++i) {
        for (size_t j = 0; j < mem_size; ++j) {
            
        }
    }

    return true;
}

void vmem_policy_optimal_destroy(void** ctx) {
    if (!VMEM_ASSERT_POLICY_CTX) return;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_optimal_context_t);
    free(c);
}

