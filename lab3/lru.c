// lru.c
#include <string.h>
#include <stdint.h>
#include "lru.h"

static void computeRates(int faults[], int n, PolicyReport *r){
    r->totalFaults = faults[n-1];
    r->rate2000 = faults[2000-1]/2000.0;
    r->rate4000 = faults[4000-1]/4000.0;
    r->rate6000 = faults[6000-1]/6000.0;
    r->rate8000 = faults[8000-1]/8000.0;
    r->rate10000= faults[10000-1]/10000.0;
}

void runLRU(const Page *refs, int n, int F, PolicyReport *out){
    FrameTable ft; ftInit(&ft, F, 1);
    int faultsSoFar = 0, faults[NREFS];
    uint32_t t = 0;

    for (int i=0;i<n;i++){
        t++;
        Page p = refs[i];
        int idx = ft.pos[p];
        if (idx >= 0){
            ft.ticks[idx] = t; // hit: update recency
        } else {
            faultsSoFar++;
            // victim: free frame else min tick
            int victim = -1;
            for (int k=0;k<F;k++) if (ft.frames[k] == -1){ victim = k; break; }
            if (victim == -1){
                uint32_t bestTick = UINT32_MAX;
                for (int k=0;k<F;k++) if (ft.ticks[k] < bestTick){ bestTick = ft.ticks[k]; victim = k; }
            }
            if (ft.frames[victim] != -1) ft.pos[ft.frames[victim]] = -1;
            ft.frames[victim] = p; ft.pos[p] = victim; ft.ticks[victim] = t;
        }
        faults[i] = faultsSoFar;
    }
    computeRates(faults, n, out);
    ftFree(&ft);
}
