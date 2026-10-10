//
//
//      policy_lfu.h
//      Alan Ksenzov 2026-10-10
//
//

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "frame.h"


typedef struct {
    // Access count per frame
    uint64_t* access_count;
} vmem_policy_lfu_context_t;


bool vmem_policy_lfu_initialize(
    uint16_t* restrict trace,
    size_t mem_size,
    void** restrict ctx
);

bool vmem_policy_lfu_on_hit(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t* restrict  frame,
    size_t                  ppn,
    void** restrict ctx
);

bool vmem_policy_lfu_on_miss(
    vmem_frame_t* restrict mem, 
    size_t                 mem_size,
    vmem_frame_t*          frame,
    size_t                 ppn,
    void** restrict ctx
);

bool vmem_policy_lfu_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn,
    void** restrict ctx
);

void vmem_policy_lfu_destroy(void** ctx);
