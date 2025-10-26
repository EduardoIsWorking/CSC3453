#include "printer.h"
#include "timeutil.h"

void printProcessLine(FILE *out, const PCB *p){
    char f[32], w[32], t[32], r[32];
    formatT2(p->finishT2, f, sizeof(f));
    formatT2(p->waitingT2, w, sizeof(w));
    formatT2(p->turnaroundT2, t, sizeof(t));
    formatT2(p->responseT2, r, sizeof(r));
    fprintf(out, "%4d %7d %9d %8s %8s %8s %9s %7d\n",
            p->pid, p->arrivalMs, p->burstMs, f, w, t, r, p->contextCount);
}

void printAveragesLine(FILE *out, const Averages *avg) {
    char ab[32], aw[32], at[32], ar[32];
    formatT2(avg->avgBurstT2, ab, sizeof(ab));
    formatT2(avg->avgWaitT2,  aw, sizeof(aw));
    formatT2(avg->avgTurnT2,  at, sizeof(at));
    formatT2(avg->avgRespT2,  ar, sizeof(ar));

    fprintf(out,
        "Average CPU burst time = %s ms,   Average waiting time = %s ms\n"
        "Average turn around time = %s ms,  Average response time = %s ms\n"
        "Total No. of Context Switching Performed = %d\n",
        ab, aw, at, ar, avg->totalContext);
}

void printHeaderFCFS(FILE *out, int n){
    (void)n;
    fprintf(out,
"*********************************************************************\n"
"************ Scheduling algorithm: FCFS ******************************\n"
"*********************************************************************\n"
"%4s %7s %9s %8s %8s %8s %9s %7s\n",
"pid","arrival","CPU-burst","finish","waiting","turn","response","Context");
}

void printHeaderRR(FILE *out, int n, int quantumMs){
    fprintf(out,
"*********************************************************************\n"
"************ Scheduling algorithm: Round Robin ***********************\n"
"************ ( No. of Task = %d  Quantum = %d ) **********************\n"
"*********************************************************************\n"
"%4s %7s %9s %8s %8s %8s %9s %7s\n",
n, quantumMs,
"pid","arrival","CPU-burst","finish","waiting","turn","response","Context");
}

void printHeaderSRTF(FILE *out, int n){
    (void)n;
    fprintf(out,
"*********************************************************************\n"
"************ Scheduling algorithm: SRTF ******************************\n"
"*********************************************************************\n"
"%4s %7s %9s %8s %8s %8s %9s %7s\n",
"pid","arrival","CPU-burst","finish","waiting","turn","response","Context");
}
