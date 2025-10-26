#include <stdlib.h>
#include "heap.h"

static void swap(void **a, void **b){ void* t=*a; *a=*b; *b=t; }

void heapInit(Heap *h, int cap, heapCmp cmp){
    h->a = (void**)malloc(sizeof(void*) * (cap>0?cap:16));
    h->size = 0;
    h->cap = (cap>0?cap:16);
    h->cmp = cmp;
}

int heapEmpty(Heap *h){ return h->size==0; }

static void siftUp(Heap *h, int i){
    while(i>0){
        int p=(i-1)/2;
        if(h->cmp(h->a[i], h->a[p])>=0) break;
        swap(&h->a[i], &h->a[p]); i=p;
    }
}

static void siftDown(Heap *h, int i){
    for(;;){
        int l=2*i+1, r=l+1, m=i;
        if(l<h->size && h->cmp(h->a[l], h->a[m])<0) m=l;
        if(r<h->size && h->cmp(h->a[r], h->a[m])<0) m=r;
        if(m==i) break;
        swap(&h->a[i], &h->a[m]); i=m;
    }
}

void heapPush(Heap *h, void *item){
    if(h->size==h->cap){
        h->cap*=2;
        h->a = (void**)realloc(h->a, sizeof(void*)*h->cap);
        if(!h->a) abort();
    }
    h->a[h->size]=item;
    siftUp(h, h->size++);
}

void* heapPop(Heap *h){
    if(h->size==0) return NULL;
    void *top = h->a[0];
    h->a[0]=h->a[--h->size];
    if(h->size>0) siftDown(h,0);
    return top;
}

void heapClear(Heap *h){
    free(h->a);
    h->a=NULL; h->size=0; h->cap=0; h->cmp=NULL;
}