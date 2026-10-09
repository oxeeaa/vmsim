//
//      Virtual memory sim
//      Alan Ksenzov (Benz) 2026-10-08
//


#include "Simulator.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define ARRAY_IMPLEMENTATION
#include "Array.h"
#include "Simulator.h"
#include "Trace.h"


void print_usage(void) {
    printf(
        "usage: vmsim [options]\n"
        "\n"
        "options:\n"
        "  -a <algorithm>   Page replacement algorithm:\n"
        "                   ideal, optimal, fifo, lru, random\n"
        "  -n <frames>      Number of physical frames where n > 0\n"
        "  -f <file>        Path to trace file\n"
        "  --help           Show this help message\n"
        "\n"
        "example:\n"
        "  vmsim -a fifo -n 4 -f trace.vmsim\n\n"
    );
}


static struct option long_options[] = {
    { "help", no_argument, 0, 'h' },
    { 0, 0, 0, 0 }
};

int main(int argc, char** argv) {
    if (argc == 1) {
        print_usage();
        return EXIT_FAILURE;
    }

    vmem_algorithm_t algo   = VMEM_NONE;
    char* trace_file        = NULL;
    uint64_t nf             = 0;

    int opt;
    while ((opt = getopt_long(argc, argv, "a:n:f:h", long_options, NULL)) != -1) {
        switch(opt) {
            case 'a':
                algo = stvmemalg(optarg);
            break;
            case 'n':
                nf = atoi(optarg);
            break;
            case 'f':
                trace_file = optarg;
            break;
            case 'h':
                print_usage();
            return EXIT_SUCCESS;
            default:
                print_usage();
            return EXIT_FAILURE;
        }
    }
    
    if (nf == 0) {
        printf("error: number of frames must be greater than 0\n\n");
        return EXIT_FAILURE;
    }

    if (!trace_file) {
        printf("error: missing trace file\n\n");
        return EXIT_FAILURE;
    }

    if (algo == VMEM_NONE) {
        printf("error: invalid algorithm\n\n");
        return EXIT_FAILURE;
    }
    
    FILE* f = fopen(trace_file, "rb");
    if (!f) {
        perror("fopen");
        printf("\n");
        return EXIT_FAILURE;
    }
    
    uint16_t* trace = NULL;
    if (trace_parse(f, &trace) != TRACE_OK) {
        printf("error: could not parse trace file\n\n");
        fclose(f);
        return EXIT_FAILURE;
    }

    if (!trace) {
        printf("error: trace file is either empty or malformed\n\n");
        fclose(f);
        return EXIT_FAILURE;
    }


    vmem_exec_result_t vmr;

    int res = vmem_execute(algo, nf, trace, &vmr);

    trace_free(trace);
    
    printf(
        "Run result: \n"
        "\tHits      %llu\n"
        "\tMisses    %llu\n"
        "\tEvictions %llu\n\n",
        vmr.hits, vmr.misses, vmr.evictions
    );

    return res;
}
