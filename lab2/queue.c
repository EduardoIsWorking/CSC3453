#include <stdlib.h>
#include "queue.h"

void queueInit(Queue *q) {
    q->head = q->tail = NULL;
    q->size = 0;
}

int queueEmpty(Queue *q) { return q->size == 0; }
int queueSize(Queue *q)  { return q->size; }

void queuePush(Queue *q, void *item) {
    QNode *n = (QNode*)malloc(sizeof(QNode));
    if (!n) abort();
    n->data = item;
    n->next = NULL;
    if (q->tail) q->tail->next = n;
    else         q->head = n;
    q->tail = n;
    q->size++;
}

void* queuePop(Queue *q) {
    if (!q->head) return NULL;
    QNode *n = q->head;
    void *item = n->data;
    q->head = n->next;
    if (!q->head) q->tail = NULL;
    free(n);
    q->size--;
    return item;
}

void queueClear(Queue *q) {
    QNode *cur = q->head;
    while (cur) {
        QNode *nx = cur->next;
        free(cur);
        cur = nx;
    }
    q->head = q->tail = NULL;
    q->size = 0;
}
