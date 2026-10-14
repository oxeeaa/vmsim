//
//
//      simulator.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stdint.h>

#include "frame.h"
#include "page.h"
#include "algorithm.h"


extern int vmem_gs_verbose_output;
extern vmem_page_t vmem_page_table[];


typedef struct {
    uint64_t hits,
             misses,
             evictions,
             vpn_count;
} vmem_execute_result_t;



int vmem_execute(
    uint16_t*              restrict trace, 
    size_t                          mem_size, 
    vmem_algorithm_t                algorithm, 
    vmem_execute_result_t* restrict pout_result
);

bool vmem_find_free_frame(
    vmem_frame_t*  restrict memory, 
    size_t                  memory_size,
    vmem_frame_t** restrict poutframe,
    size_t*        restrict poutppn
);
