#include <stdlib.h>
#include <string.h>
#include "pager.h"
#include "fifo.h"
#include "lru.h"
#include "opt.h"

void ftInit(FrameTable *ft, int F, int useTicks){
    ft->frameCount = F;
    ft->frames = (int*)malloc(sizeof(int)*F);
    ft->ticks  = useTicks ? (uint32_t*)malloc(sizeof(uint32_t)*F) : NULL;
    for (int i=0;i<F;i++){ ft->frames[i] = -1; if (ft->ticks) ft->ticks[i]=0; }
    for (int p=0;p<MAX_PAGES;p++) ft->pos[p] = -1;
    ft->fifoNext = 0;
}

void ftFree(FrameTable *ft){
    free(ft->frames); ft->frames=NULL;
    if (ft->ticks){ free(ft->ticks); ft->ticks=NULL; }
}

void simulatePolicy(Policy alg, const Page *refs, int n, int F,
                    const int *nextIndexOPT, PolicyReport *out){
    switch(alg){
        case ALG_FIFO: runFIFO(refs, n, F, out); break;
        case ALG_LRU:  runLRU(refs, n, F, out);  break;
        case ALG_OPT:  runOPT(refs, nextIndexOPT, n, F, out); break;
    }
}
