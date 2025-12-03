#include <stdio.h>
#include "parser.h"

int readRefs(const char *path, Page *out, int nExpected){
    FILE *f = fopen(path, "r");
    if (!f) return 1;
    int i=0, x;
    while (i<nExpected && fscanf(f, "%d", &x)==1){
        if (x<0 || x>=MAX_PAGES){ fclose(f); return 2; }
        out[i++] = (Page)x;
    }
    fclose(f);
    return (i==nExpected)? 0 : 3;
}
