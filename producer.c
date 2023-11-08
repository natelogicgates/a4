#ifndef PRODUCER_C
#define PRODUCER_C

#include "producer.h"

void *producer(void *arg) {
    struct thread_args *marg = (struct thread_args*)(arg);
    RequestQueue *queue = marg->sh->queue;

    while (1) {
        usleep(marg->wait_ms);
        pthread_mutex_lock(&marg->sh->producer_lock);

        marg->sh->requests_produced++;
        if(marg->sh->max_requests < marg->sh->requests_produced) {
            enqueue(queue, End);
            pthread_mutex_unlock(&marg->sh->producer_lock);
            break;
        }

        if(marg->mrType == Bitcoin && marg->sh->produced[marg->mrType] == 6) {
            pthread_mutex_unlock(&marg->sh->producer_lock);
            sem_wait(&marg->sh->sem_btc_cap);
            pthread_mutex_lock(&marg->sh->producer_lock);
        }

        enqueue(queue, marg->mrType);
        marg->sh->produced[marg->mrType]++;

        pthread_mutex_lock(&marg->sh->in_requestQueue_lock);
        marg->sh->inRequestQueue[marg->mrType]++;
        pthread_mutex_unlock(&marg->sh->in_requestQueue_lock);


        // Report the addition of the request to stdout
        report_request_added(marg->mrType, marg->sh->produced, marg->sh->inRequestQueue);

        pthread_mutex_unlock(&marg->sh->producer_lock);
    }

    pthread_exit(NULL);
}

#endif // PRODUCER_C
