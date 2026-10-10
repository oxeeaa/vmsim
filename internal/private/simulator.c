
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"
#include "algorithm.h"
#include "array.h"
#include "memory.h"
#include "page.h"
#include "policy.h"
#include "frame.h"
#include "print.h"


int vmem_gs_verbose_output = 0;


int vmem_execute(
    uint16_t*              restrict trace, 
    size_t                          mem_size, 
    vmem_algorithm_t                algorithm, 
    vmem_execute_result_t* restrict pout_result
) {
    FILE* print_stream = stdout;
    vmem_policy_descriptor_t policy;
    void* policy_ctx = NULL;

    int get_policy_res = 0;
    if ((get_policy_res = vmem_get_policy_descriptor(algorithm, &policy)) != 0) {
        if (get_policy_res == 1) {
            errno = ENOSYS;
        } else {
            errno = EINVAL;
        }

        perror("vmem_get_policy_descriptor");
        return EXIT_FAILURE;
    }
    
    if (!policy.initialize(trace, mem_size, &policy_ctx)) {
        return EXIT_FAILURE;
    }

    vmem_frame_t* mem = calloc(mem_size, sizeof(*mem));
    if (!mem) 
        return EXIT_FAILURE;

    if (vmem_gs_verbose_output) {
        fprintf(print_stream, "\n VMSIM 1.67alpha nightly-build\n");
        fprintf(print_stream, " running virtual memory simulation..\n\n");
        vmem_print_header(print_stream);
    }

    for (size_t i = 0; i < ARRAY_SIZE(trace); ++i) {
        uint16_t        vaddr = trace[i];
        size_t          vpn   = vaddr / VMEM_PAGE_SZ;
        vmem_page_t*    page  = &vmem_page_table[vpn];
        
        if (page->state == VMEM_PAGE_STATE_RESIDENT) {
            if (vmem_gs_verbose_output)
                vmem_print_row(print_stream, vaddr, vpn, "hit", NULL);

            pout_result->hits++;

            policy.on_hit(mem, mem_size, &mem[page->ppn], page->ppn, &policy_ctx);
            continue;
        }

        //
        // Not resident - try to fetch a frame
        //
        vmem_frame_t* frame;
        size_t        ppn;
        if (vmem_find_free_frame(mem, mem_size, &frame, &ppn)) {
            if (vmem_gs_verbose_output)
                vmem_print_row(print_stream, vaddr, vpn, "miss", NULL);
            //
            // Cold miss
            //
            pout_result->misses++;
            policy.on_miss(mem, mem_size, frame, ppn, &policy_ctx);
        } else {
            //
            // Somebody needs to go
            //
            pout_result->evictions++;
            pout_result->misses++;
            policy.select_victim(mem, mem_size, &frame, &ppn, &policy_ctx);

            if (vmem_gs_verbose_output) {
                char details[128];
                snprintf(details, sizeof(details), "evicted vpn %u in frame %zu", frame->vpn, ppn);
                vmem_print_row(print_stream, vaddr, vpn, "swap", details);
            }

            //
            // Invalidate current vpn mapped to this victim frame
            //
            vmem_page_table[frame->vpn].state = VMEM_PAGE_STATE_INVALID;
            //
            // Run on miss logic after victim selected
            //
            policy.on_miss(mem, mem_size, frame, ppn, &policy_ctx);
        }
        //
        // Mark page resident and update frame vpn
        //
        frame->is_occupied = true;
        frame->vpn         = vpn;
        page->state        = VMEM_PAGE_STATE_RESIDENT;
        page->ppn          = ppn;
    }
    
    if (vmem_gs_verbose_output)
        fprintf(print_stream, "\n");

    policy.destroy(&policy_ctx);

    free(mem);

    return EXIT_SUCCESS;
}

bool vmem_find_free_frame(
    vmem_frame_t*  restrict memory, 
    size_t                  memory_size,
    vmem_frame_t** restrict poutframe,
    size_t*        restrict poutppn
) {
    for (size_t i = 0; i < memory_size; ++i) {
        if (!memory[i].is_occupied) {
            *poutppn = i;
            *poutframe = &memory[i];
            return true;
        }
    }

    return false;
}
