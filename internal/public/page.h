//
//
//      page.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stddef.h>


typedef enum {
    VMEM_PAGE_STATE_INVALID,
    VMEM_PAGE_STATE_RESIDENT,
} vmem_page_state_t;


typedef struct {
    size_t            ppn;
    vmem_page_state_t state;
    bool              has_been_seen;
} vmem_page_t;
