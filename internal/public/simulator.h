//
//
//      simulator.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stdint.h>

#include "page.h"


typedef struct {
    uint64_t hits,
             misses,
             evictions;
} vmem_execute_result_t;

static vmem_page_t vmem_page_table[10];

int     vmem_execute(uint16_t* trace);

bool    vmem_find_free_frame(
    // frame_t* memory, 
    // size_t memory_size,
    // frame_t** restrict poutframe,
    // size_t *poutppn
);
