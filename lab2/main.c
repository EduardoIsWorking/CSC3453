#include <stdio.h>
#include <stdlib.h>
#include "sim.h"
#include "queue.h"
#include "timeutil.h"
#include "parser.h"
#include "algFCFS.h"

static void printUsage(const char *prog) {
    fprintf(stderr,
        "Usage:\n"
        "  %s <input> <output> 0                # FCFS\n"
        "  %s <input> <output> 1 <quantum_ms>   # RR\n"
        "  %s <input> <output> 2                # SRTF\n",
        prog, prog, prog);
}

int main(int argc, char **argv) {
    if (argc < 4) { printUsage(argv[0]); return 1; }

    const char *inPath  = argv[1];
    const char *outPath = argv[2];
    int algo = atoi(argv[3]);
    int quantumMs = 0;

    if (algo == 1) {
        if (argc < 5) { printUsage(argv[0]); return 1; }
        quantumMs = atoi(argv[4]);
        if (quantumMs <= 0) {
            fprintf(stderr, "quantum must be > 0\n");
            return 1;
        }
    }

    PCB *pcbs = NULL;
    int n = 0;
    int parseErr = parseInput(inPath, &pcbs, &n);
    if (parseErr != 0) {
        fprintf(stderr, "Input parse error (%d)\n", parseErr);
        return 1;
    }

    int rc = 0;
    if (algo == 0) {
        rc = runFCFS(pcbs, n, outPath);
    } else if (algo == 1) {
        fprintf(stderr, "Round Robin not implemented yet\n");
        rc = 3;
    } else if (algo == 2) {
        fprintf(stderr, "SRTF not implemented yet\n");
        rc = 4;
    } else {
        printUsage(argv[0]);
        rc = 2;
    }

    free(pcbs);
    return rc;
}
