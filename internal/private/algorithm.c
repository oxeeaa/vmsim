
#include "algorithm.h"
#include <string.h>


vmem_algorithm_t vmem_stoalg(const char* s) {
    if (strcmp(s, "random") == 0) {
        return VMEM_RANDOM;
    } else if (strcmp(s, "ideal") == 0)
        return VMEM_IDEAL;
    else if (strcmp(s, "fifo") == 0)
        return VMEM_FIFO;
    else if (strcmp(s, "lru") == 0)
        return VMEM_LRU;
    else if (strcmp(s, "optimal") == 0)
        return VMEM_OPTIMAL;

    return VMEM_INVALID;
}
