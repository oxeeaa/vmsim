//
//
//      memory.h
//      Alan Ksenzov 2026-10-09
//
//

#pragma once

#include <stdint.h>


#define VMEM_VADDR_SPACE_SIZE UINT16_MAX
#define VMEM_PAGE_SZ          256
#define VMEM_VP_COUNT         (VMEM_VADDR_SPACE_SIZE / VMEM_PAGE_SZ + 1)
