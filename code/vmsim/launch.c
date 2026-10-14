//
//
//      Virtual memory sim
//      Alan Ksenzov (Benz) 2026-10-08
//
//


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

#define ARRAY_IMPLEMENTATION
#include "array.h"
#define TRACE_IMPLEMENTATION
#include "trace.h"
#include "simulator.h"
#include "algorithm.h"


void print_usage(void) {
    printf(
        "usage: vmsim [options]\n"
        "\n"
        "options:\n"
        "  -a <algorithm>   Page replacement algorithm:\n"
        "                   clock, optimal, fifo, lru, mru, lfu, random\n"
        "  -n <frames>      Number of physical frames where n > 0\n"
        "  -f <file>        Path to trace file\n"
        "  --verbose        Print detailed output for each address access\n"
        "  --help           Show this help message\n"
        "\n"
        "example:\n"
        "  vmsim -a fifo -n 4 -f trace.vmsim\n\n"
    );
}


static struct option long_options[] = {
    { "help", no_argument, 0, 'h' },
    {  "verbose", no_argument, 0, 'v'},
    { 0, 0, 0, 0 }
};

int main(int argc, char** argv) {
    if (argc == 1) {
        print_usage();
        return EXIT_FAILURE;
    }

    vmem_algorithm_t algo       = VMEM_INVALID;
    char*            trace_file = NULL;
    uint64_t         nf         = 0;

    int opt;
    while ((opt = getopt_long(argc, argv, "a:n:f:h:v", long_options, NULL)) != -1) {
        switch(opt) {
            case 'a':
                algo = vmem_stoa(optarg);
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
            case 'v':
                vmem_gs_verbose_output = 1;
            break;
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

    if (algo == VMEM_INVALID) {
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
    
    fclose(f);
    
    // reminder to self
    // tv_sec = seconds 
    // tv_nsec = nanoseconds
    // { 2, 250000000 } = 2.250000000s
    struct timespec start;
    struct timespec end;

    timespec_get(&start, TIME_UTC);

    vmem_execute_result_t vmr = { 0 };
    int res = vmem_execute(trace, nf, algo, &vmr);
    
    timespec_get(&end, TIME_UTC);

    double elapsed_ms = 
        (double)(end.tv_sec - start.tv_sec) * 1000.0 +
        (double)(end.tv_nsec - start.tv_nsec) / 1000000.0;


    if (res == EXIT_SUCCESS)
        printf(
            " finished in %.6fms \n"
            " summary:\n"
            "   - algortihm       %s\n"
            "   - frames          %llu\n"
            "   - traces          %zu\n"
            "  ───────────────────────\n"
            "   - hits            %llu\n"
            "   - misses          %llu\n"
            "   - swaps           %llu\n\n",
            elapsed_ms,
            vmem_atoa(algo), nf, ARRAY_SIZE(trace), vmr.hits, vmr.misses, vmr.evictions
        );

    trace_free(trace);

    return res;
}
