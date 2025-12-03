#ifndef PAGER_H
#define PAGER_H
#include <stddef.h>
#include <stdint.h>

#define MAX_PAGES 10000
#define NREFS     10000
#define INF_NEXT  2000000000

typedef uint16_t Page;

typedef struct {
    int totalFaults;
    double rate2000;
    double rate4000;
    double rate6000;
    double rate8000;
    double rate10000;
} PolicyReport;

typedef enum { ALG_FIFO=0, ALG_LRU=1, ALG_OPT=2 } Policy;

typedef struct {
    int frameCount;     // F
    int *frames;        // size F, -1 empty or page number
    int pos[MAX_PAGES]; // page -> frame index or -1
    uint32_t *ticks;    // LRU: size F, last used time; NULL for other algos
    int fifoNext;       // FIFO pointer [0..F-1]
} FrameTable;

// Common simulate entry
void simulatePolicy(Policy alg, const Page *refs, int n, int frameCount,
                    const int *nextIndexOPT, PolicyReport *out);

// Helpers
void ftInit(FrameTable *ft, int F, int useTicks);
void ftFree(FrameTable *ft);

#endif
