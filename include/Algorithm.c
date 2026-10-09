#include "Algorithm.h"
#include "PageTable.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>




DEFINE_VMEM_RESOLVE_ALG_FN(lru) {
    return 0;
}

DEFINE_VMEM_RESOLVE_ALG_FN(ideal) {
    return 0;
}

DEFINE_VMEM_RESOLVE_ALG_FN(optimal) {
    return 0;
}

DEFINE_VMEM_RESOLVE_ALG_FN(fifo) {
    vmem_fifo_ctx_t* fifo = ctx;
    if (!fifo || !fifo->loaded_at || !nf) {
        return false;
    }

    size_t oldest = 0;

    for (size_t i = 1; i < nf; ++i) {
        if (fifo->loaded_at[i] < fifo->loaded_at[oldest])
            oldest = i;
    }

    *poutppn = oldest;
    *poutframe = &fs[oldest];

    return true;
}

DEFINE_VMEM_RESOLVE_ALG_FN(random) {

    *poutppn = (size_t)(rand() % nf);
    *poutframe = &fs[*poutppn];

    printf("Evicting frame %zu\n", *poutppn);

    return true;
}
