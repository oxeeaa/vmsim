//
//
//      algorithm.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stdbool.h>

#include "policy.h"


typedef enum {
    VMEM_INVALID = 0,
    VMEM_FIFO,
    VMEM_OPTIMAL,
    VMEM_LRU,
    VMEM_RANDOM,
} vmem_algorithm_t;

// typedef enum {
// } vmem_algorithm_context_t;

// Converts string to vmem_algorithm_t.
vmem_algorithm_t vmem_stoalg(const char* s);

// Gets corresponding policy descriptor for given algorithm.
vmem_policy_descriptor_t vmem_get_policy_descriptor(vmem_algorithm_t al);
