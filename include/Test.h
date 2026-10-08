//
//  Test
//  Alan Ksenzov (Benz) 2026-10-06
//


#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "Colors.h"


typedef struct {
    bool(*test_fn)(void);
    const char* test_name;
} test_t;

// Runs test fn, puts results into fd.
// Returns result of fn function.
bool testb(bool(*fn)(void), FILE* restrict out, const char* restrict test_name);

#ifdef TEST_IMPLEMENTATION

bool testb(bool(*fn)(void), FILE* restrict out, const char* restrict test_name) {
    fprintf(
        out, 
        AC_BG_BLACK
        " "
        AC_RESET
        " running"
        " %s\n", test_name
    );
    
    // reminder to self
    // tv_sec = seconds 
    // tv_nsec = nanoseconds
    // { 2, 250000000 } = 2.250000000s
    struct timespec start;
    struct timespec end;

    timespec_get(&start, TIME_UTC);
    bool is_ok = fn();
    timespec_get(&end, TIME_UTC);
    
    double elapsed_ms = 
        (double)(end.tv_sec - start.tv_sec) * 1000.0 +
        (double)(end.tv_nsec - start.tv_nsec) / 1000000.0;


    if (is_ok) {
        fprintf(
            out,
            AC_FG_BLACK
            AC_BG_GREEN
            " " 
            AC_RESET
            " + passed"
            " %.6f ms\n",
            elapsed_ms
        );
    } else {
        fprintf(
            out,
            AC_FG_BLACK
            AC_BG_RED 
            " " 
            AC_RESET
            " - failed"
            " %.6f ms\n",
            elapsed_ms
        );
    }

    return is_ok;
}


#endif
