#ifndef PRINTER_H
#define PRINTER_H
#include <stdio.h>
#include "sim.h"
#include "metrics.h"

void printProcessLine(FILE *out, const PCB *p);
void printAveragesLine(FILE *out, const Averages *avg);
void printHeaderFCFS(FILE *out, int n);
void printHeaderRR(FILE *out, int n, int quantumMs);
void printHeaderSRTF(FILE *out, int n);

#endif
