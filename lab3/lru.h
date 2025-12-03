// lru.h
#ifndef LRU_H
#define LRU_H
#include "pager.h"
void runLRU(const Page *refs, int n, int F, PolicyReport *out);
#endif
