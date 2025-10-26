#ifndef ALG_RR_H
#define ALG_RR_H

#include "sim.h"

// Run the RR scheduler with quantumMs
// Writes per-process results and averages to outPath
// Returns 0 on success
int runRR(PCB *pcbs, int n, const char *outPath, int quantumMs);

#endif