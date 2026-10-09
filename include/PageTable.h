//
//      PageTable.h
//      Alan Ksenzov (Benz) 2026-10-09
//

#pragma once


#include <stddef.h>
#include <stdint.h>


#define VMEM_SIZE       1024 * 64
#define MEM_FRAME_SZ    256
#define VPN_COUNT       (VMEM_SIZE / MEM_FRAME_SZ + 1)

typedef enum {
    PAGE_INVALID    = 0,
    PAGE_RESIDENT,
    PAGE_SWAPPED,
} page_state_t;

typedef struct {
    uint64_t        ppn;
    page_state_t    state;
} page_entry_t;

typedef struct {
    uint16_t    vpn;
    bool        is_occupied;
} frame_t;

// Decides what physical page to evict.
// Params:
//      fs - frame stream
//      nf - frame stream size
//      trace - trace stream
//      trace_len - trace stream size
//      current - current vaddr idx
//      void* ctx - relevant algorithm context (Nullable)
// Returns ppn of "to be evicted" page
typedef bool (*resolve_fn_t) (
    frame_t* restrict fs,
    size_t nf,
    const uint16_t* restrict trace,
    size_t trace_len,
    size_t current,
    void* ctx,
    frame_t** restrict poutframe,
    size_t* restrict poutppn
);

#define DEFINE_VMEM_RESOLVE_ALG_FN(name) bool vmem_resolve_alg_##name(  \
    frame_t* restrict fs,                                         \
    size_t nf,                                                          \
    const uint16_t* restrict trace,                                     \
    size_t trace_len,                                                   \
    size_t current,                                                     \
    void* ctx,                                                          \
    frame_t** restrict poutframe,                                       \
    size_t* restrict poutppn                                            \
)                                                          

static page_entry_t vmem_page_table[VPN_COUNT] = {0};

// Get physical page number, pout, mapped to virtual page number by address.
// Returns false if vaddr is out of range
bool    vmem_page_lookup(uint16_t vaddr, page_entry_t** pout);

bool    vmem_find_free_frame(frame_t* fs, size_t nf, frame_t** restrict poutframe, size_t* poutppn);
