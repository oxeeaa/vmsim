//
//
//      algorithm.c
//      Alan Ksenzov 2026-10-11
//
//


#include "algorithm.h"
#include <string.h>


vmem_algorithm_t vmem_stoa(const char* s) {
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

const char* vmem_atoa(vmem_algorithm_t al) {
    switch (al) {
        case VMEM_OPTIMAL:  return "OPTIMAL";
        case VMEM_RANDOM:   return "RANDOM";
        case VMEM_FIFO:     return "FIFO";
        case VMEM_LRU:      return "LRU";
        case VMEM_LFU:      return "LFU";
        case VMEM_MRU:      return "MRU";
        case VMEM_CLOCK:    return "CLOCK";
        case VMEM_INVALID:
        default:
            return "INVALID";
    }
}
