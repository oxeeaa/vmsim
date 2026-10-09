
#include <errno.h>
#include <stdlib.h>
#include <time.h>

#include "policy_random.h"


bool vmem_policy_random_initialize(uint16_t* _) {
    srand((unsigned int)time(NULL));

    return true;
}

bool vmem_policy_random_on_hit() {
    return true;
}

bool vmem_policy_random_on_miss() {
    return true;
}

bool vmem_policy_random_select_victim(
    vmem_frame_t* restrict mem, 
    size_t mem_size, 
    vmem_frame_t** restrict pout_frame, 
    size_t* pout_ppn
) {
    if (
        mem == NULL || 
        mem_size == 0 ||
        pout_ppn == NULL ||
        pout_frame == NULL
    ) {
        errno = EINVAL;
        return false;
    }

    size_t ppn = (size_t)rand() % mem_size;

    *pout_frame = &mem[ppn];
    *pout_ppn   = ppn;

    return true;
}
