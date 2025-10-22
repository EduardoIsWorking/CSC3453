#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"
#include "sim.h"
#include "timeutil.h"

static int isCommentOrBlank(const char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    return (*s == '\0' || *s == '#');
}

int parseInput(const char *path, PCB **out, int *n) {
    *out = NULL; *n = 0;
    FILE *f = fopen(path, "r");
    if (!f) return 1;

    int cap = 16;
    PCB *arr = (PCB*)malloc(sizeof(PCB) * cap);
    if (!arr) { fclose(f); return 2; }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), f)) {
        if (isCommentOrBlank(line)) continue;

        int pid, arrMs, burstMs;
        if (sscanf(line, "%d %d %d", &pid, &arrMs, &burstMs) != 3) {
            fclose(f); free(arr); return 3;
        }
        if (burstMs < 0 || arrMs < 0) { fclose(f); free(arr); return 4; }

        if (count == cap) {
            cap *= 2;
            PCB *tmp = (PCB*)realloc(arr, sizeof(PCB) * cap);
            if (!tmp) { fclose(f); free(arr); return 5; }
            arr = tmp;
        }
        pcbInit(&arr[count], pid, arrMs, burstMs);
        count++;
    }

    fclose(f);
    if (count == 0) { free(arr); return 6; }

    *out = arr;
    *n = count;
    return 0;
}
