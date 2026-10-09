//
//
//      frame.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stdint.h>


typedef struct {
    uint16_t vpn;
    bool     is_occupied;
} vmem_frame_t;
