#include <stdio.h>
#include <stdlib.h>
#include "algSRTF.h"
#include "heap.h"
#include "timeutil.h"
#include "metrics.h"
#include "printer.h"

static int cmpArrival(const void *a, const void *b){
    const PCB *pa=(const PCB*)a, *pb=(const PCB*)b;
    if(pa->arrivalT2 != pb->arrivalT2) return (pa->arrivalT2<pb->arrivalT2)?-1:1;
    if(pa->pid != pb->pid) return (pa->pid<pb->pid)?-1:1;
    return 0;
}

// Heap stores PCB*, but our Heap uses void** with comparator on items,
// so we pass PCB* directly and compare as PCB*:
static int cmpSrtf(const void *a, const void *b){
    const PCB *pa=(const PCB*)a, *pb=(const PCB*)b;
    if (pa->remainingT2 != pb->remainingT2) return (pa->remainingT2 < pb->remainingT2)?-1:1;
    if (pa->arrivalT2   != pb->arrivalT2)   return (pa->arrivalT2   < pb->arrivalT2)?-1:1;
    if (pa->pid         != pb->pid)         return (pa->pid         < pb->pid)?-1:1;
    return 0;
}

int runSRTF(PCB *pcbs, int n, const char *outPath){
    // Copy and sort by arrival
    PCB *arr=(PCB*)malloc(sizeof(PCB)*n);
    if(!arr) return 1;
    for(int i=0;i<n;i++) arr[i]=pcbs[i];
    qsort(arr, n, sizeof(PCB), cmpArrival);

    Sim sim; simInit(&sim);
    Heap ready; heapInit(&ready, n, cmpSrtf);

    int i=0;           // next arrival index
    int finished=0;
    PCB *running=NULL;

    while(finished<n){
        // Admit arrivals at now
        while(i<n && arr[i].arrivalT2<=sim.nowT2){
            accountWaitStart(&arr[i], arr[i].arrivalT2);
            heapPush(&ready, &arr[i]); // store PCB*; comparator handles PCB*
            i++;
        }

        // If nothing ready
        if(!running && heapEmpty(&ready)){
            if(i<n){ sim.nowT2=arr[i].arrivalT2; continue; }
            else break;
        }

        // Dispatch if idle
        if(!running){
            running = (PCB*)heapPop(&ready);
            accountWaitStop(running, sim.nowT2);
            markResponseIfFirst(running, sim.nowT2);
        }

        // Next events
        t2_t tComplete = sim.nowT2 + running->remainingT2;
        t2_t tNextArr  = (i<n)? arr[i].arrivalT2 : (t2_t)((1ULL<<62)-1);

        if(tNextArr < tComplete){
            // Run until next arrival
            t2_t dt = tNextArr - sim.nowT2;
            running->remainingT2 -= dt;
            sim.nowT2 = tNextArr;

            // Admit all arrivals at this time
            while(i<n && arr[i].arrivalT2==sim.nowT2){
                accountWaitStart(&arr[i], arr[i].arrivalT2);
                heapPush(&ready, &arr[i]);
                i++;
            }

            // Check for preemption: top of heap has strictly smaller remaining
            if(!heapEmpty(&ready)){
                PCB *best = (PCB*)ready.a[0]; // peek root
                if(best->remainingT2 < running->remainingT2){
                    // Preempt running
                    running->contextCount++;
                    sim.totalContext++;
                    accountWaitStart(running, sim.nowT2);
                    heapPush(&ready, running);
                    addCtxOverhead(&sim); // +0.5 ms exactly
                    running = NULL; // will dispatch next loop
                }
            }
            // Else continue with current running
        } else {
            // Finish before or exactly at next arrival
            sim.nowT2 = tComplete;
            running->remainingT2 = 0;
            finishProcess(&sim, running, sim.nowT2);
            running = NULL;
            finished++;

            // If arrivals also happen exactly now, they will be admitted next loop.
        }
    }

    // Output
    FILE *out = fopen(outPath,"w");
    if (!out) { heapClear(&ready); free(arr); return 2; }
    printHeaderSRTF(out, n);

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
    free(arr);
    heapClear(&ready);
    return 0;
}
