//
//      Simulator.c
//      Alan Ksenzov (Benz) 2026-10-09
//
//

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


#include "Simulator.h"
#include "Array.h"
#include "PageTable.h"
#include "Algorithm.h"
#include "Simulator.h"



vmem_algorithm_t stvmemalg(const char* s) {
    if (strcmp(s, "ideal") == 0)
        return VMEM_IDEAL;
    else if (strcmp(s, "optimal") == 0)
        return VMEM_OPTIMAL;
    else if (strcmp(s, "fifo") == 0)
        return VMEM_FIFO;
    else if (strcmp(s, "lru") == 0)
        return VMEM_LRU;
    else if (strcmp(s, "random") == 0)
        return VMEM_RANDOM;
    else 
        return VMEM_NONE;
}


resolve_fn_t vmem_pick_resolve_alg(vmem_algorithm_t al) {
    switch (al) {
        case VMEM_IDEAL:
            return vmem_resolve_alg_ideal;
        case VMEM_OPTIMAL:
            return vmem_resolve_alg_optimal;
        case VMEM_FIFO:
            return vmem_resolve_alg_fifo;
        case VMEM_LRU:
            return vmem_resolve_alg_lru;
        case VMEM_RANDOM:
            return vmem_resolve_alg_random;
        case VMEM_NONE:
        default:
            return NULL;
    }
}

// vmempage -> mempage
// an address is inside of a page
// 0x0000 is in first page
// 0x0100 is in second page
// 0x0200 is in third page etc.
// Whenever i access a page i want to check if the page is present
// if not - i need to load it
// if it is not present and there is no space for this page i need to evict the page
//
// I need to know whether a slot is occupied i.e. a page is loaded at the current addr
// What addresses 

typedef struct {
    bool        is_mapped;
} page_table_entry_t;

int vmem_execute(vmem_algorithm_t al, uint64_t nf, uint16_t* ts, vmem_exec_result_t* presout) {
    
    //
    //  Convert addr to vpn
    //  Lookup page table[vpn]
    //  Check table[vpn].state
    //  if PAGE_RESIDENT, count a hit and notify the replacement algorithm
    //  Otherwise find an empty frame; if none exists, ask replacement alg which frame to evict
    //  Update evicted pages state, assign new page to the frame, and update page table.
    //
    
    srand(time(NULL));

    uint64_t hits = 0,
             misses = 0,
             evictions = 0;
    
    resolve_fn_t resolve = vmem_pick_resolve_alg(al);   
    if (!resolve || !nf) {
        errno = EINVAL;
        perror("vmem_pick_resolve_alg");

        return EXIT_FAILURE;
    }
    
    frame_t* frames = calloc(nf, sizeof(*frames));
    uint64_t* loaded_at = calloc(nf, sizeof(*loaded_at));
    uint64_t* last_used = calloc(nf, sizeof(*last_used));
    if (!frames || !loaded_at || !last_used) {
        perror("calloc");

        free(frames);
        free(loaded_at);
        free(last_used);
        return EXIT_FAILURE;
    }

    vmem_fifo_ctx_t fifo_ctx = { 
        .loaded_at = loaded_at,
        .clock = 0
    };
    vmem_lru_ctx_t lru_ctx = { .last_used = last_used };

    void* ctx = NULL;

    switch (al) {
        case VMEM_IDEAL:
        break;
        case VMEM_FIFO:
            ctx = &fifo_ctx;
        break;
        case VMEM_LRU:
            ctx = &lru_ctx;
        break;
        case VMEM_OPTIMAL:
        case VMEM_RANDOM:
        case VMEM_NONE:
        default:
    }

    for (size_t i = 0; i < ARRAY_SIZE(ts); ++i) {
        uint16_t addr = ts[i];
        uint64_t vpn = addr / MEM_FRAME_SZ;
        page_entry_t* page = NULL;

        if (!vmem_page_lookup(addr, &page)) {
            errno = EINVAL;
            perror("vmem_page_lookup");
            return EXIT_FAILURE;
        }
        
        if (page->state == PAGE_RESIDENT) {
            ++hits;
            continue;
        }
        
        ++misses;

        frame_t* frame = NULL;
        size_t ppn = 0;
        if (vmem_find_free_frame(frames, nf, &frame, &ppn)) {
            frame->is_occupied = true;
            
            if (al == VMEM_FIFO) {
                fifo_ctx.loaded_at[ppn] = fifo_ctx.clock++;
            }

        } else {
            // Somebody needs to go
            ++evictions;
            
            if (!resolve(frames, nf, ts, ARRAY_SIZE(ts), i, ctx, &frame, &ppn)) {
                // Could not resolve?
                errno = EINVAL;
                perror("resolve");

                // Mem leak
                return EXIT_FAILURE;
            }
            
            if (al == VMEM_FIFO)
                fifo_ctx.loaded_at[ppn] = fifo_ctx.clock++;

            // Invalidate evicted page
            uint16_t victim_vpn = frame->vpn;
            page_entry_t* victim_page = &vmem_page_table[victim_vpn];

            victim_page->state = PAGE_INVALID;
        }

        frame->vpn = vpn;
        page->state = PAGE_RESIDENT;
        page->ppn = ppn;
    }
    
    presout->evictions = evictions;
    presout->hits = hits;
    presout->misses = misses;

    return EXIT_SUCCESS;
}

bool vmem_page_lookup(uint16_t vaddr, page_entry_t** pout) {
    if (!pout) return false;

    *pout = &vmem_page_table[vaddr / MEM_FRAME_SZ];
    return true;
}
