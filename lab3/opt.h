// opt.h
#ifndef OPT_H
#define OPT_H
#include "pager.h"
void buildNextIndex(const Page *refs, int n, int *nextIndex); // length n
void runOPT(const Page *refs, const int *nextIndex, int n, int F, PolicyReport *out);
#endif
