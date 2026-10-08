#include <stdlib.h>
#include <string.h>


#include "Solution.h"



vmem_algorithm_t stvmemalg(const char* s) {
    if (strcmp(s, "ideal") == 0)
        return VMEM_IDEAL;
    else if (strcmp(s, "optimal") == 0)
        return VMEM_OPTIMAL;
    else if (strcmp(s, "fifo") == 0)
        return VMEM_FIFO;
    else if (strcmp(s, "lru") == 0)
        return VMEM_LRU;
    else 
        return VMEM_NONE;
}


int vmem_run(vmem_algorithm_t al, uint64_t nf, uint16_t* ts) {
    return 0;
    // I am not storing anything so there actually is no need to
    // allocate anything.
    //
    // Only a page table is needed?
}
