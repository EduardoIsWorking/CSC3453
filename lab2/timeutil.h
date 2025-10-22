#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <stdint.h>
#include <stdio.h>

typedef int64_t t2_t;                 // 1 unit = 0.5 ms

static inline t2_t msToT2(int ms)     { return (t2_t)ms * 2; }
static inline int  t2WholeMs(t2_t t2) { return (int)(t2 / 2); }
static inline int  t2HasHalf(t2_t t2) { return (int)(t2 & 1); }

// Write "X.0" or "X.5" to buf
static inline void formatT2(t2_t t2, char *buf, size_t n) {
    int ms = t2WholeMs(t2);
    snprintf(buf, n, "%d.%c", ms, t2HasHalf(t2) ? '5' : '0');
}

#endif
