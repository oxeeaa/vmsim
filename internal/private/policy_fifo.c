

#include "policy_fifo.h"


bool vmem_policy_fifo_initialize(
    uint16_t* trace,
    size_t mem_size,
    void** restrict ctx
) {}
bool vmem_policy_fifo_on_hit(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    void** restrict ctx
) {}

bool vmem_policy_fifo_on_miss(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    void** restrict ctx
) {}

bool vmem_policy_fifo_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn,
    void** restrict ctx
) {}


void vmem_policy_fifo_destroy(void** _) {}
