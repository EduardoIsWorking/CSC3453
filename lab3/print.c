#include <stdio.h>
#include "print.h"

void printTableHeader(FILE *f, int F){
    fprintf(f,
"===============================================\n"
" Page Replacement Algorithm Simulation (frame size = %d)\n"
"===============================================\n"
" Page fault rates\n"
" Algorithm  Total page faults   2000    4000    6000    8000    10000\n"
"---------------------------------------------------------------------\n", F);
}

void printPolicyLine(FILE *f, const char *name, const PolicyReport *r){
    fprintf(f, "%-6s %10d %10.3f %7.3f %7.3f %7.3f %9.3f\n",
        name, r->totalFaults,
        r->rate2000, r->rate4000, r->rate6000, r->rate8000, r->rate10000);
}
