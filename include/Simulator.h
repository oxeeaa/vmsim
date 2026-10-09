//
//      Simulator.h
//      Alan Ksenzov (Benz) 2026-10-09
//

#pragma once

#include <stdint.h>
#include <stddef.h>

#include "PageTable.h"


typedef struct {
    uint64_t hits,
             misses,
             evictions;
} vmem_exec_result_t;

typedef enum {
    VMEM_NONE = 0,
    VMEM_IDEAL,
    VMEM_OPTIMAL,
    VMEM_FIFO,
    VMEM_LRU,
    VMEM_RANDOM,
} vmem_algorithm_t;

// String to virtual memory eviction algorithm
vmem_algorithm_t    stvmemalg(const char* s);

// Picks suitable resolve algorithm.
// Returns null if could not pick an algorithm or al is VMEM_NONE
resolve_fn_t        vmem_pick_resolve_alg(vmem_algorithm_t al);

// Simulates virtual memory access patterns.
// Params:
//      al - memory access algorithm
//      nf - number of physical frames
//      tf - trace array
//      presout - pointer to result
int                 vmem_execute(vmem_algorithm_t al, uint64_t nf, uint16_t* ts, vmem_exec_result_t* presout);
