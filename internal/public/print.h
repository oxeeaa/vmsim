//
//
//      print.h
//      Alan Ksenzov 2026-10-10
//
//

#pragma once

#include <stdint.h>
#include <stdio.h>


void vmem_print_row(
    FILE*       restrict stream,
    uint16_t             addr,
    uint8_t              vpn,
    const char* restrict type,
    const char* restrict details
);

void vmem_print_header(FILE* stream);
