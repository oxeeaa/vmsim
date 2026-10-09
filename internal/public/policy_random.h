//
//
//      policy_random.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "frame.h"


bool vmem_policy_random_initialize(uint16_t* trace);

bool vmem_policy_random_on_hit(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn
);

bool vmem_policy_random_on_miss(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn
);

bool vmem_policy_random_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn
);
