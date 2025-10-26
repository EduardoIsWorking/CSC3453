#ifndef HEAP_H
#define HEAP_H

typedef int (*heapCmp)(const void*, const void*);

typedef struct Heap {
    void **a;
    int size;
    int cap;
    heapCmp cmp;
} Heap;

void heapInit(Heap *h, int cap, heapCmp cmp);
int  heapEmpty(Heap *h);
void heapPush(Heap *h, void *item);
void* heapPop(Heap *h);       // NULL if empty
void heapClear(Heap *h);      // frees array (not items)

#endif