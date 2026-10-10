//
//
//      policy.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "algorithm.h"
#include "frame.h"


#define VMEM_DEFINE_POLICY_CTX(name, type) type* name = (type*)(*ctx);
#define VMEM_ASSERT_POLICY_CTX (ctx && *ctx)


typedef struct {
    // Initializes policy defined by vmem_algorithm_t
    // Params:
    //          - trace: trace array
    //          - mem_size: size of memory
    //          - ctx: policy context (if any)
    bool(*initialize)(uint16_t* restrict trace, size_t mem_size, void** restrict ctx);
    // Params:
    //          - mem: physical memory
    //          - mem_size: physical memory size
    //          - frame: loaded frame
    //          - ppn: loaded ppn
    //          - ctx: policy context (if needed)
    bool(*on_hit)(
        vmem_frame_t* restrict  mem, 
        size_t                  mem_size,
        vmem_frame_t* restrict  frame,
        size_t                  ppn,
        void** restrict         ctx
    );
    // Params:
    //          - mem: physical memory
    //          - mem_size: physical memory size
    //          - frame: loaded frame
    //          - ppn: loaded ppn
    //          - ctx: policy context (if needed)
    bool(*on_miss)(
        vmem_frame_t* restrict  mem, 
        size_t                  mem_size,
        vmem_frame_t*           frame,
        size_t                  ppn,
        void** restrict         ctx
    );
    // Selects a frame to be evicted.
    // Params:
    //          - mem: physical memory
    //          - mem_size: physical memory size
    //          - pout_frame: frame to be evicted.
    //          - pout_ppn: physical page number of the victim frame.
    //          - ctx: policy context (if needed)
    bool(*select_victim)(
        vmem_frame_t* restrict  mem, 
        size_t                  mem_size, 
        vmem_frame_t** restrict pout_frame, 
        size_t* restrict        pout_ppn,
        void** restrict         ctx
    );
    // Destroy the policy context if there is any
    // Params:
    //          - ctx: policy context (if any)
    void(*destroy)(void** ctx);
} vmem_policy_descriptor_t;

// Gets corresponding policy descriptor for given algorithm.
// Returns 0 on success, -1 for invalid algorithm, 1 for unimplemented algorithm.
int vmem_get_policy_descriptor(vmem_algorithm_t al, vmem_policy_descriptor_t* pout_policy);
