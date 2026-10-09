
#include <stdint.h>
#include <stdlib.h>

#include "simulator.h"
#include "algorithm.h"
#include "array.h"
#include "memory.h"
#include "page.h"
#include "policy.h"
#include "frame.h"


int vmem_execute(
    uint16_t*              restrict trace, 
    size_t                          mem_size, 
    vmem_algorithm_t                algorithm, 
    vmem_execute_result_t* restrict pout_result
) {
    vmem_policy_descriptor_t policy;
    if (vmem_get_policy_descriptor(algorithm, &policy) != 0) {
        return EXIT_FAILURE;
    }
    
    if (!policy.initialize(trace)) {
        return EXIT_FAILURE;
    }

    vmem_frame_t* mem = calloc(mem_size, sizeof(*mem));
    if (!mem) 
        return EXIT_FAILURE;
    
    for (size_t i = 0; i < ARRAY_SIZE(trace); ++i) {
        uint16_t        vaddr = trace[i];
        size_t          vpn   = vaddr / VMEM_PAGE_SZ;
        vmem_page_t*    page  = &vmem_page_table[vpn];
        
        if (page->state == VMEM_PAGE_STATE_RESIDENT) {
            pout_result->hits++;

            policy.on_hit();
            continue;
        }

        //
        // Not resident - try to fetch a frame
        //
        vmem_frame_t* frame;
        size_t        ppn;
        if (vmem_find_free_frame(mem, mem_size, &frame, &ppn) == 0) {
            //
            // Cold miss
            //
            pout_result->misses++;
            policy.on_miss();
        } else {
            //
            // Somebody needs to go
            //
            pout_result->evictions++;
            pout_result->misses++;
            policy.select_victim(mem, mem_size, &frame, &ppn);
            //
            // Invalidate current vpn mapped to this victim frame
            //
            vmem_page_table[frame->vpn].state = VMEM_PAGE_STATE_INVALID;
        }
        //
        // Mark page resident and update frame vpn
        //
        frame->is_occupied = true;
        frame->vpn         = vpn;
        page->state        = VMEM_PAGE_STATE_RESIDENT;
        page->ppn          = ppn;
    }
    
    free(mem);

    return EXIT_SUCCESS;
}
