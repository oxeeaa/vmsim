//
//
//      algorithm.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stdbool.h>


typedef enum {
    VMEM_INVALID = 0,
    VMEM_FIFO,
    VMEM_OPTIMAL,
    VMEM_LRU,
    VMEM_RANDOM,
    VMEM_CLOCK,
} vmem_algorithm_t;

// typedef enum {
// } vmem_algorithm_context_t;

// Converts string to vmem_algorithm_t.
vmem_algorithm_t vmem_stoalg(const char* s);

