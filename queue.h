#ifndef QUEUE_H
#define QUEUE_H


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "tradecrypto.h"

#define MAX_QUEUE_SIZE 15

typedef struct {
    RequestType requests[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty, not_full;
} RequestQueue;

void init_request_queue(RequestQueue *queue) ;
void enqueue(RequestQueue *queue, RequestType request) ;
RequestType dequeue(RequestQueue *queue) ;
void destroy_request_queue(RequestQueue *queue) ;
int is_full(const RequestQueue *queue);
int is_empty(const RequestQueue *queue);

#endif // QUEUE_H
