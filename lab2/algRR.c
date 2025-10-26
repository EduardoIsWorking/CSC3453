#include <stdio.h>
#include <stdlib.h>
#include "algRR.h"
#include "queue.h"
#include "timeutil.h"
#include "metrics.h"
#include "printer.h"

static int cmpArrival(const void *a, const void *b) {
    const PCB *pa = (const PCB*)a, *pb = (const PCB*)b;
    if (pa->arrivalT2 < pb->arrivalT2) return -1;
    if (pa->arrivalT2 > pb->arrivalT2) return 1;
    if (pa->pid < pb->pid) return -1;
    if (pa->pid > pb->pid) return 1;
    return 0;
}

int runRR(PCB *pcbs, int n, const char *outPath, int quantumMs) {
    t2_t qT2 = msToT2(quantumMs);

    // Work on a copy
    PCB *arr = (PCB*)malloc(sizeof(PCB)*n);
    if (!arr) return 1;
    for (int i=0;i<n;i++) arr[i] = pcbs[i];
    qsort(arr, n, sizeof(PCB), cmpArrival);

    Sim sim; simInit(&sim);
    Queue rrQueue; queueInit(&rrQueue);

    int nextArrival = 0;
    int finished = 0;

    while (finished < n) {
        // Admit arrivals up to now
        while (nextArrival < n && arr[nextArrival].arrivalT2 <= sim.nowT2) {
            accountWaitStart(&arr[nextArrival], arr[nextArrival].arrivalT2);
            queuePush(&rrQueue, &arr[nextArrival]);
            nextArrival++;
        }

        // If idle and empty, jump to next arrival
        if (queueEmpty(&rrQueue)) {
            if (nextArrival < n) {
                sim.nowT2 = arr[nextArrival].arrivalT2;
                continue;
            } else break;
        }

        // Pick next in FCFS order
        PCB *p = (PCB*)queuePop(&rrQueue);
        accountWaitStop(p, sim.nowT2);
        markResponseIfFirst(p, sim.nowT2);

        // Compute this time slice
        t2_t slice = (p->remainingT2 < qT2) ? p->remainingT2 : qT2;
        t2_t endT2 = sim.nowT2 + slice;

        // Enqueue any arrivals during the slice (<= endT2)
        while (nextArrival < n && arr[nextArrival].arrivalT2 <= endT2) {
            accountWaitStart(&arr[nextArrival], arr[nextArrival].arrivalT2);
            queuePush(&rrQueue, &arr[nextArrival]);
            nextArrival++;
        }

        // Advance time to end of slice
        sim.nowT2 = endT2;
        p->remainingT2 -= slice;

        if (p->remainingT2 == 0) {
            // Completed. No context-switch charge.
            finishProcess(&sim, p, sim.nowT2);
            finished++;
        } else {
            // Quantum expired. Preempt and return to ready.
            p->contextCount++;
            sim.totalContext++;
            accountWaitStart(p, sim.nowT2);      // back to ready at this time
            queuePush(&rrQueue, p);
            addCtxOverhead(&sim);                // exact +0.5 ms
        }
    }

    // Output
    FILE *out = fopen(outPath,"w");
    if (!out) { queueClear(&rrQueue); free(arr); return 2; }
    printHeaderRR(out, n, quantumMs);

    for (int k=0;k<n;k++){
        PCB *p=NULL;
        for (int j=0;j<n;j++) if (pcbs[k].pid==arr[j].pid){ p=&arr[j]; break; }
        finalizeMetrics(p);
        printProcessLine(out, p);
    }

    Averages avg;
    calcAverages(arr, n, sim.totalContext, &avg);
    printAveragesLine(out, &avg);
    fclose(out);
    queueClear(&rrQueue);
    free(arr);
    return 0;
}
