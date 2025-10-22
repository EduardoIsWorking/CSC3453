#ifndef SIM_H
#define SIM_H

#include <stddef.h>
#include <stdbool.h>
#include "timeutil.h"

typedef struct PCB {
    int   pid;
    int   arrivalMs;
    int   burstMs;

    t2_t  arrivalT2;
    t2_t  remainingT2;
    t2_t  startT2;        // -1 if never started
    t2_t  finishT2;

    t2_t  lastEnqT2;      // for waiting accumulation
    t2_t  waitingT2;
    t2_t  responseT2;
    t2_t  turnaroundT2;

    int   contextCount;   // preemptions that went back to ready
    int   inReady;        // boolean
} PCB;

typedef struct Sim {
    t2_t  nowT2;
    bool  cpuBusy;
    PCB  *running;
    t2_t  ctxOverheadT2;  // = 1 (0.5 ms)
    int   totalContext;
} Sim;

// lifecycle
void simInit(Sim *sim);
void pcbInit(PCB *p, int pid, int arrivalMs, int burstMs);

// accounting helpers
void markResponseIfFirst(PCB *p, t2_t nowT2);
void accountWaitStart(PCB *p, t2_t nowT2);
void accountWaitStop(PCB *p, t2_t nowT2);
void finishProcess(Sim *sim, PCB *p, t2_t nowT2);

// context switching
void addCtxOverhead(Sim *sim);   // +0.5 ms exactly and counts nothing by itself

#endif
