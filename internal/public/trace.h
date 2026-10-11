//
//
//      trace.h
//      Alan Ksenzov 2026-10-08
//
//

#pragma once


#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TRACE_EEMPTY,
    TRACE_ENOMEM,
    TRACE_OK
} trace_result_t;



trace_result_t trace_parse(FILE* f, uint16_t** pout);


#ifdef TRACE_IMPLEMENTATION

#include "array.h"

trace_result_t trace_parse(FILE* f, uint16_t** pout) {
    uint16_t* trace = (uint16_t*)array_new(10, sizeof(uint16_t));
    if (!trace) {
        return TRACE_ENOMEM;
    }

    // Go through file
    // Ox0000 separated by newlines -> can use fgets
    //
    char entry[16];
    while (fgets(entry, sizeof(entry), f)) {
        uint16_t addr = (uint16_t)strtoull(entry, NULL, 16);

        trace = (uint16_t*)array_append(trace, &addr);
        if (!trace) {
            return TRACE_ENOMEM;
        }
    }
    
    *pout = trace;

    return TRACE_OK;
}

void trace_free(uint16_t* trace) {
    if (!trace) return;

    array_delete(trace);
}

#endif // TRACE_IMPLEMENTATION
