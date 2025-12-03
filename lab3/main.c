#include <stdio.h>
#include <stdlib.h>
#include "pager.h"
#include "parser.h"
#include "print.h"
#include "opt.h"

int main(int argc, char **argv){
    if (argc != 4){
        fprintf(stderr, "Usage: %s <frame_size> <input> <output>\n", argv[0]);
        return 1;
    }
    int F = atoi(argv[1]);
    const char *inPath = argv[2], *outPath = argv[3];
    if (F<=0){ fprintf(stderr,"frame_size must be >0\n"); return 1; }

    Page refs[NREFS];
    if (readRefs(inPath, refs, NREFS)!=0){ fprintf(stderr,"bad input\n"); return 1; }

    // OPT preprocessing
    int nextIndex[NREFS];
    buildNextIndex(refs, NREFS, nextIndex);

    PolicyReport fifoR, lruR, optR;
    simulatePolicy(ALG_FIFO, refs, NREFS, F, nextIndex, &fifoR);
    simulatePolicy(ALG_LRU,  refs, NREFS, F, nextIndex, &lruR);
    simulatePolicy(ALG_OPT,  refs, NREFS, F, nextIndex, &optR);

    FILE *out = fopen(outPath, "w");
    if (!out) return 1;
    printTableHeader(out, F);
    printPolicyLine(out, "FIFO", &fifoR);
    printPolicyLine(out, "LRU",  &lruR);
    printPolicyLine(out, "OPT",  &optR);
    fclose(out);
    return 0;
}
