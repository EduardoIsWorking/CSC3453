#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct QNode {
    void *data;
    struct QNode *next;
} QNode;

typedef struct Queue {
    QNode *head;
    QNode *tail;
    int    size;
} Queue;

void queueInit(Queue *q);
int  queueEmpty(Queue *q);
int  queueSize(Queue *q);
void queuePush(Queue *q, void *item); // O(1)
void* queuePop(Queue *q);             // O(1), NULL if empty
void queueClear(Queue *q);            // frees nodes only

#endif
