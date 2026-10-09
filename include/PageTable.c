#include <stddef.h>
#include <stdint.h>

#include "PageTable.h"



bool     vmem_find_free_frame(frame_t* fs, size_t nf, frame_t** restrict poutframe, size_t* poutppn) {
    if (!fs) return -1;

    for (size_t i = 0; i < nf; ++i) {
        if (!fs[i].is_occupied) {
            *poutframe = &fs[i];
            *poutppn = i;
            return true;
        }
    }

    return false;
}
