//
//      Solution.h
//      Alan Ksenzov (Benz) 2026-10-08
//


#pragma once


#include <stddef.h>
#include <stdint.h>


#define VMEM_SIZE       1024 * 64
#define MEM_FRAME_SZ    256

typedef enum {
    VMEM_NONE = 0,
    VMEM_IDEAL,
    VMEM_OPTIMAL,
    VMEM_FIFO,
    VMEM_LRU,
} vmem_algorithm_t;


vmem_algorithm_t stvmemalg(const char* s);

// Simulates virtual memory access patterns.
// Params:
//      al - memory access algorithm
//      nf - number of physical frames
//      tf - trace array
int vmem_run(vmem_algorithm_t al, uint64_t nf, uint16_t* ts);
