#ifndef ARGS_H
#define ARGS_H
#include "queue.h"

struct thread_args_shared {
    sem_t sem_btc_cap;

    RequestQueue *queue;
    unsigned int *inRequestQueue, *produced, **consumed;

    pthread_mutex_t in_requestQueue_lock, producer_lock, consumer_lock;

    int max_requests, requests_produced;
    sem_t barrier;
};

struct thread_args {
    struct thread_args_shared *sh;
    int wait_ms;
    RequestType mrType;
    ConsumerType mcType;
};

#endif // ARGS_H
