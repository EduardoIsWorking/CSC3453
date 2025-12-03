#ifndef PRINT_H
#define PRINT_H
#include <stdio.h>
#include "pager.h"

void printTableHeader(FILE *f, int frameCount);
void printPolicyLine(FILE *f, const char *name, const PolicyReport *r);
#endif
