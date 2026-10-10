//
//
//      policy_clock.h
//      Alan Ksenzov 2026-10-10
//
//

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "frame.h"


typedef struct {
    // Reference bit per physical frame
    bool*  ref_bits;
    // Clock hand sweeping from 0 to physical mem size - 1
    size_t hand;
} vmem_policy_clock_context_t;


bool vmem_policy_clock_initialize(
    uint16_t* restrict trace,
    size_t mem_size,
    void** restrict ctx
);

bool vmem_policy_clock_on_hit(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t* restrict  frame,
    size_t                  ppn,
    void** restrict ctx
);

bool vmem_policy_clock_on_miss(
    vmem_frame_t* restrict mem, 
    size_t                 mem_size,
    vmem_frame_t*          frame,
    size_t                 ppn,
    void** restrict ctx
);

bool vmem_policy_clock_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn,
    void** restrict ctx
);

void vmem_policy_clock_destroy(void** ctx);
