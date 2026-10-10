
#include "print.h"
#include <stdio.h>


void vmem_print_row(
    FILE*       restrict stream,
    uint16_t             addr,
    uint8_t              vpn,
    const char* restrict type,
    const char* restrict details
) {
    fprintf(
        stream,
        " 0x%04X  %-5u  %-8s  %s\n",
        addr, vpn, type, details ? details : ""
    );
}


void vmem_print_header(FILE* stream) {
    fprintf(
        stream,
        " %-6s  %-5s  %-8s  %s\n",
        "addr", "vpn", "type", "details"
    );
    
    fprintf(stream, "--------------------------------------------------\n");
}
