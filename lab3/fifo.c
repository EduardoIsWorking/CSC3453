// fifo.c
#include <string.h>
#include "fifo.h"

static void computeRates(int faults[], int n, PolicyReport *r){
    r->totalFaults = faults[n-1];
    r->rate2000 = faults[2000-1] / 2000.0;
    r->rate4000 = faults[4000-1] / 4000.0;
    r->rate6000 = faults[6000-1] / 6000.0;
    r->rate8000 = faults[8000-1] / 8000.0;
    r->rate10000= faults[10000-1]/10000.0;
}

void runFIFO(const Page *refs, int n, int F, PolicyReport *out){
    FrameTable ft; ftInit(&ft, F, 0);
    int faultsSoFar = 0;
    int faults[NREFS]; // n==10000

    for (int i=0;i<n;i++){
        Page p = refs[i];
        int fidx = ft.pos[p];
        if (fidx >= 0){
            // hit: nothing
        } else {
            // fault
            faultsSoFar++;
            // find victim: free frame if any else ft.fifoNext
            int victim = -1;
            for (int k=0;k<F;k++) if (ft.frames[k] == -1){ victim = k; break; }
            if (victim == -1){ victim = ft.fifoNext; }
            // evict
            if (ft.frames[victim] != -1) ft.pos[ft.frames[victim]] = -1;
            // place
            ft.frames[victim] = p;
            ft.pos[p] = victim;
            // advance pointer if we filled the fifo slot
            if (victim == ft.fifoNext) ft.fifoNext = (ft.fifoNext + 1) % F;
        }
        faults[i] = faultsSoFar;
    }
    computeRates(faults, n, out);
    ftFree(&ft);
}
