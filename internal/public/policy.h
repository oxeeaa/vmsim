//
//
//      policy.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once


#include <cstdint>


typedef struct {
    bool(*on_hit)();
    bool(*on_miss)();
    bool(*select_victim)();
} vmem_policy_descriptor_t;

typedef struct {
    uint16_t vpn;
    bool     is_occupied;
} vmem_frame_t;
