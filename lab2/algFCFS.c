#include <stdio.h>
#include <stdlib.h>
#include "algFCFS.h"
#include "queue.h"
#include "timeutil.h"

// Compare by arrival time, then PID for tie-breaking
static int cmpArrival(const void *a, const void *b) {
    const PCB *pa = (const PCB*)a;
    const PCB *pb = (const PCB*)b;
    if (pa->arrivalT2 < pb->arrivalT2) return -1;
    if (pa->arrivalT2 > pb->arrivalT2) return 1;
    if (pa->pid < pb->pid) return -1;
    if (pa->pid > pb->pid) return 1;
    return 0;
}

int runFCFS(PCB *pcbs, int n, const char *outPath) {
    // Copy array so original order is untouched
    PCB *arr = (PCB*)malloc(sizeof(PCB) * n);
    if (!arr) return 1;
    for (int i = 0; i < n; i++) arr[i] = pcbs[i];
    qsort(arr, n, sizeof(PCB), cmpArrival);

    Sim sim;
    simInit(&sim);
    Queue fcfsQueue;
    queueInit(&fcfsQueue);

    int nextArrival = 0;
    int finished = 0;

    while (finished < n) {
        // Enqueue arrivals ready at current time
        while (nextArrival < n && arr[nextArrival].arrivalT2 <= sim.nowT2) {
            accountWaitStart(&arr[nextArrival], arr[nextArrival].arrivalT2);
            queuePush(&fcfsQueue, &arr[nextArrival]);
            nextArrival++;
        }

        // If CPU idle and queue empty, jump to next arrival
        if (queueEmpty(&fcfsQueue)) {
            if (nextArrival < n) {
                sim.nowT2 = arr[nextArrival].arrivalT2;
                continue;
            } else break;
        }

        // Dequeue next process
        PCB *p = (PCB*)queuePop(&fcfsQueue);
        accountWaitStop(p, sim.nowT2);
        markResponseIfFirst(p, sim.nowT2);

        // Run to completion (non-preemptive)
        t2_t endT2 = sim.nowT2 + p->remainingT2;

        // Add any arrivals that occur during execution
        while (nextArrival < n && arr[nextArrival].arrivalT2 <= endT2) {
            accountWaitStart(&arr[nextArrival], arr[nextArrival].arrivalT2);
            queuePush(&fcfsQueue, &arr[nextArrival]);
            nextArrival++;
        }

        sim.nowT2 = endT2;
        p->remainingT2 = 0;
        finishProcess(&sim, p, sim.nowT2);
        finished++;
    }

    // Write results
    FILE *out = fopen(outPath, "w");
    if (!out) { free(arr); return 2; }

    t2_t sumBurst = 0, sumWait = 0, sumTurn = 0, sumResp = 0;
    for (int k = 0; k < n; k++) {
        PCB *p = NULL;
        for (int j = 0; j < n; j++)
            if (pcbs[k].pid == arr[j].pid) { p = &arr[j]; break; }

        char fbuf[32], wbuf[32], tbuf[32], rbuf[32];
        formatT2(p->finishT2,    fbuf, sizeof(fbuf));
        formatT2(p->waitingT2,   wbuf, sizeof(wbuf));
        formatT2(p->turnaroundT2,tbuf, sizeof(tbuf));
        formatT2(p->responseT2,  rbuf, sizeof(rbuf));

        fprintf(out,
            "PID %d  finish=%s  wait=%s  turn=%s  resp=%s  ctx=%d\n",
            p->pid, fbuf, wbuf, tbuf, rbuf, p->contextCount);

        sumBurst += msToT2(p->burstMs);
        sumWait  += p->waitingT2;
        sumTurn  += p->turnaroundT2;
        sumResp  += p->responseT2;
    }

    char ab[32], aw[32], at[32], ar[32];
    formatT2(sumBurst / n, ab, sizeof(ab));
    formatT2(sumWait  / n, aw, sizeof(aw));
    formatT2(sumTurn  / n, at, sizeof(at));
    formatT2(sumResp  / n, ar, sizeof(ar));

    fprintf(out,
        "\nAverages: burst=%s  wait=%s  turn=%s  resp=%s  total_ctx=%d\n",
        ab, aw, at, ar, sim.totalContext);

    fclose(out);
    free(arr);
    queueClear(&fcfsQueue);
    return 0;
}