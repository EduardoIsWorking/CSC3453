#include <string.h>
#include "opt.h"

void buildNextIndex(const Page *refs, int n, int *nextIndex) {
    // nextIndex[i] = next occurrence index of refs[i], or -1 if none
    int nextPosForPage[MAX_PAGES];
    for (int p = 0; p < MAX_PAGES; p++) {
        nextPosForPage[p] = -1;
    }

    for (int i = n - 1; i >= 0; i--) {
        Page pg = refs[i];
        nextIndex[i] = nextPosForPage[pg];
        nextPosForPage[pg] = i;
    }
}

static void computeRates(int faults[], int n, PolicyReport *r) {
    r->totalFaults = faults[n - 1];
    r->rate2000    = faults[2000  - 1] / 2000.0;
    r->rate4000    = faults[4000  - 1] / 4000.0;
    r->rate6000    = faults[6000  - 1] / 6000.0;
    r->rate8000    = faults[8000  - 1] / 8000.0;
    r->rate10000   = faults[10000 - 1] / 10000.0;
}

// OPT using precomputed per-reference nextIndex and per-page future pointers
void runOPT(const Page *refs, const int *nextIndex, int n, int F, PolicyReport *out) {
    FrameTable ft;
    ftInit(&ft, F, 0);

    int faultsSoFar = 0;
    int faults[NREFS];

    // firstPos[p] = first occurrence index of page p, or -1
    int firstPos[MAX_PAGES];
    for (int p = 0; p < MAX_PAGES; p++) {
        firstPos[p] = -1;
    }
    for (int i = 0; i < n; i++) {
        Page pg = refs[i];
        if (firstPos[pg] == -1) {
            firstPos[pg] = i;
        }
    }

    // curNext[p] = next use index of page p at current time, or -1 if none
    int curNext[MAX_PAGES];
    for (int p = 0; p < MAX_PAGES; p++) {
        curNext[p] = firstPos[p];
    }

    for (int i = 0; i < n; i++) {
        Page pg = refs[i];
        int idx = ft.pos[pg];

        if (idx >= 0) {
            // Hit: nothing to change in frame table
        } else {
            // Page fault
            faultsSoFar++;

            int victim = -1;

            // Try to find a free frame first
            for (int k = 0; k < F; k++) {
                if (ft.frames[k] == -1) {
                    victim = k;
                    break;
                }
            }

            if (victim == -1) {
                // No free frame: evict the page whose next use is farthest in the future
                int bestScore = -1;
                int bestK = 0;

                for (int k = 0; k < F; k++) {
                    Page q = (Page)ft.frames[k];
                    int nextUse = curNext[q]; // -1 means "never used again"
                    int score = (nextUse == -1) ? INF_NEXT : nextUse;
                    if (score > bestScore) {
                        bestScore = score;
                        bestK = k;
                    }
                }
                victim = bestK;
            }

            // Evict old page if any
            if (ft.frames[victim] != -1) {
                Page old = (Page)ft.frames[victim];
                ft.pos[old] = -1;
            }

            // Place new page
            ft.frames[victim] = pg;
            ft.pos[pg] = victim;
        }

        faults[i] = faultsSoFar;

        // Advance future pointer for the current page: we just used refs[i]
        if (curNext[pg] == i) {
            curNext[pg] = nextIndex[i];  // nextIndex[i] is its next occurrence or -1
        }
    }

    computeRates(faults, n, out);
    ftFree(&ft);
}
