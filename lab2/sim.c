#include <string.h>
#include "sim.h"

void simInit(Sim *sim) {
    sim->nowT2 = 0;
    sim->cpuBusy = false;
    sim->running = NULL;
    sim->ctxOverheadT2 = 1;  // 0.5 ms
    sim->totalContext = 0;
}

void pcbInit(PCB *p, int pid, int arrivalMs, int burstMs) {
    memset(p, 0, sizeof(*p));
    p->pid = pid;
    p->arrivalMs = arrivalMs;
    p->burstMs = burstMs;

    p->arrivalT2   = msToT2(arrivalMs);
    p->remainingT2 = msToT2(burstMs);
    p->startT2     = (t2_t)-1;
    p->finishT2    = (t2_t)0;

    p->lastEnqT2   = (t2_t)0;
    p->waitingT2   = (t2_t)0;
    p->responseT2  = (t2_t)0;
    p->turnaroundT2= (t2_t)0;

    p->contextCount= 0;
    p->inReady     = 0;
}

void markResponseIfFirst(PCB *p, t2_t nowT2) {
    if (p->startT2 == (t2_t)-1) {
        p->startT2 = nowT2;
        p->responseT2 = nowT2 - p->arrivalT2;
    }
}

void accountWaitStart(PCB *p, t2_t nowT2) {
    p->inReady = 1;
    p->lastEnqT2 = nowT2;
}

void accountWaitStop(PCB *p, t2_t nowT2) {
    if (p->inReady) {
        if (nowT2 > p->lastEnqT2) p->waitingT2 += nowT2 - p->lastEnqT2;
        p->inReady = 0;
    }
}

void finishProcess(Sim *sim, PCB *p, t2_t nowT2) {
    (void)sim; // mark unused
    p->finishT2 = nowT2;
    p->turnaroundT2 = p->finishT2 - p->arrivalT2;
}

void addCtxOverhead(Sim *sim) {
    sim->nowT2 += sim->ctxOverheadT2; // exact +0.5 ms
}
