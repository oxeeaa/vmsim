//
//
//      algorithm.c
//      Alan Ksenzov 2026-10-11
//
//


#include "algorithm.h"
#include <string.h>


vmem_algorithm_t vmem_stoalg(const char* s) {
    if (strcmp(s, "random") == 0) {
        return VMEM_RANDOM;
    } else if (strcmp(s, "clock") == 0)
        return VMEM_CLOCK;
    else if (strcmp(s, "fifo") == 0)
        return VMEM_FIFO;
    else if (strcmp(s, "lru") == 0)
        return VMEM_LRU;
    else if (strcmp(s, "optimal") == 0)
        return VMEM_OPTIMAL;
    else if (strcmp(s, "mru") == 0)
        return VMEM_MRU;
    else if (strcmp(s, "lfu") == 0)
        return VMEM_LFU;
    return VMEM_INVALID;
}
