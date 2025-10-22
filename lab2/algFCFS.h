#ifndef ALG_FCFS_H
#define ALG_FCFS_H

#include "sim.h"

// Run the FCFS scheduler on the given processes
// Writes per-process results and averages to outPath
// Returns 0 on success
int runFCFS(PCB *pcbs, int n, const char *outPath);

#endif