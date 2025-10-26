#include "metrics.h"

void finalizeMetrics(PCB *p) {
    if (p->finishT2 && !p->turnaroundT2)
        p->turnaroundT2 = p->finishT2 - p->arrivalT2;
    if (p->startT2 != (t2_t)-1 && !p->responseT2)
        p->responseT2 = p->startT2 - p->arrivalT2;
    if (p->inReady) {
        // If somehow still marked in ready, stop waiting at finish
        p->waitingT2 += (p->finishT2 - p->lastEnqT2);
        p->inReady = 0;
    }
}

void calcAverages(PCB *pcbs, int n, int simTotalContext, Averages *out) {
    t2_t sumBurst=0, sumWait=0, sumTurn=0, sumResp=0;
    for (int i=0;i<n;i++) {
        PCB *p = &pcbs[i];
        finalizeMetrics(p);
        sumBurst += (t2_t) (2 * p->burstMs);  // ms -> t2
        sumWait  += p->waitingT2;
        sumTurn  += p->turnaroundT2;
        sumResp  += p->responseT2;
    }
    out->avgBurstT2 = sumBurst / n;
    out->avgWaitT2  = sumWait  / n;
    out->avgTurnT2  = sumTurn  / n;
    out->avgRespT2  = sumResp  / n;
    out->totalContext = simTotalContext;
}