//
//
//      policy.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "frame.h"


typedef struct {
    // Initializes policy defined by vmem_algorithm_t
    // Params:
    //          - trace: trace array
    bool(*initialize)(uint16_t* trace);
    bool(*on_hit)();
    bool(*on_miss)();
    // Selects a frame to be evicted.
    // Params:
    //          - mem: physical memory
    //          - mem_size: physical memory size
    //          - pout_frame: frame to be evicted.
    //          - pout_ppn: physical page number of the victim frame.
    bool(*select_victim)(
        vmem_frame_t* restrict  mem, 
        size_t                  mem_size, 
        vmem_frame_t** restrict pout_frame, 
        size_t*                 pout_ppn
    );
} vmem_policy_descriptor_t;

