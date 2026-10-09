#pragma once

#include <stdint.h>

#include "PageTable.h"


typedef struct {
    uint64_t*   loaded_at;
    uint64_t    clock;
} vmem_fifo_ctx_t;

typedef struct {
    uint64_t* last_used;
    uint64_t  clock;
} vmem_lru_ctx_t;


DEFINE_VMEM_RESOLVE_ALG_FN(lru);
DEFINE_VMEM_RESOLVE_ALG_FN(ideal);
DEFINE_VMEM_RESOLVE_ALG_FN(optimal);
DEFINE_VMEM_RESOLVE_ALG_FN(fifo);
DEFINE_VMEM_RESOLVE_ALG_FN(random);
