//
//
//      policy_clock.c
//      Alan Ksenzov 2026-10-11
//
//


#include <stdlib.h>

#include "policy.h"
#include "policy_clock.h"


bool vmem_policy_clock_initialize(
    uint16_t* restrict trace,
    size_t mem_size,
    void** restrict ctx
) {
    (void)trace;

    if (!ctx) return false;

    vmem_policy_clock_context_t* c = malloc(sizeof(*c));
    if (!c)
        return false;

    c->ref_bit = calloc(mem_size, sizeof(*c->ref_bit));
    if (!c->ref_bit) {
        free(c);
        return false;
    }

    c->hand = 0;

    *ctx = (void*)c;

    return true;
}

bool vmem_policy_clock_on_hit(
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
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_clock_context_t);

    //
    // Give seconds chance; mark frame as recently referenced
    //
    c->ref_bit[ppn] = true;

    return true;
}

bool vmem_policy_clock_on_miss(
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
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_clock_context_t);

    //
    // Newly loaded frame starst with reference bit 0
    //
    c->ref_bit[ppn] = false;

    return true;
}

bool vmem_policy_clock_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn,
    void** restrict ctx
) {
    if (!VMEM_ASSERT_POLICY_CTX || !pout_ppn || !pout_frame)
        return false;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_clock_context_t);

    //
    // Sweep clock hand untill I find a frame with reference 0
    //
    while (c->ref_bit[c->hand]) {
        //
        // Clear reference bitt
        c->ref_bit[c->hand] = false;
        //
        // Advance clock hand circularly
        c->hand = (c->hand + 1) % mem_size;
    }

    //
    // VICTIM FOOOOUND
    //
    size_t victim_ppn = c->hand;

    //
    // Advance clock hand past the victim
    //
    c->hand = (c->hand + 1) % mem_size;

    *pout_ppn   = victim_ppn;
    *pout_frame = &mem[victim_ppn];

    return true;
}

void vmem_policy_clock_destroy(void** ctx) {
    if (!VMEM_ASSERT_POLICY_CTX) return;
    VMEM_DEFINE_POLICY_CTX(c, vmem_policy_clock_context_t);

    free(c->ref_bit);
    free(c);
}
