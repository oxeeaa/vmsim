//
//
//      policy_lru.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "frame.h"


typedef struct {
    uint64_t* last_used;
    uint64_t  clock;
} vmem_policy_lru_context_t;


bool vmem_policy_lru_initialize(
    uint16_t* restrict trace,
    size_t mem_size,
    void** restrict ctx
);

bool vmem_policy_lru_on_hit(
    vmem_frame_t* restrict mem, 
    size_t mem_size,
    vmem_frame_t* restrict  frame,
    size_t                  ppn, 
    void** restrict ctx
);

bool vmem_policy_lru_on_miss(
    vmem_frame_t* restrict mem, 
    size_t                 mem_size, 
    vmem_frame_t*          frame,
    size_t                 ppn,
    void** restrict ctx
);

bool vmem_policy_lru_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn,
    void** restrict ctx
);

void vmem_policy_lru_destroy(void** ctx);

