//
//
//      policy_optimal.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "frame.h"


typedef struct {
    uint16_t* trace;
    size_t    trace_size;
    size_t    current_idx;
} vmem_policy_optimal_context_t;


bool vmem_policy_optimal_initialize(
    uint16_t* restrict trace,
    size_t             mem_size,
    void**    restrict ctx
);

bool vmem_policy_optimal_on_hit(
    vmem_frame_t* restrict mem, 
    size_t                 mem_size, 
    vmem_frame_t* restrict frame,
    size_t                 ppn,
    void**        restrict ctx
);

bool vmem_policy_optimal_on_miss(
    vmem_frame_t* restrict mem, 
    size_t                 mem_size, 
    vmem_frame_t* restrict frame,
    size_t                 ppn,
    void**        restrict ctx
);

bool vmem_policy_optimal_select_victim(
    vmem_frame_t*  restrict mem, 
    size_t                  mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t*        restrict pout_ppn,
    void**         restrict ctx
);

void vmem_policy_optimal_destroy(void** ctx);
