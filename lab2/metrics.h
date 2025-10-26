#ifndef METRICS_H
#define METRICS_H

#include "sim.h"   // PCB, t2_t

typedef struct Averages {
    t2_t avgBurstT2;
    t2_t avgWaitT2;
    t2_t avgTurnT2;
    t2_t avgRespT2;
    int  totalContext;   // from Sim (or sum of per-process if you prefer)
} Averages;

// Defensive completion for a single PCB (safe if already set)
void finalizeMetrics(PCB *p);

// Compute averages for an array of PCBs; pass simTotalContext from the run
void calcAverages(PCB *pcbs, int n, int simTotalContext, Averages *out);

#endif