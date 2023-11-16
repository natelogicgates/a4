#include "consumer.h"

void *consumer(void *arg) {
    struct thread_args *marg = (struct thread_args*)(arg);
    RequestQueue *queue = marg->sh->queue;

    while (1) {
        pthread_mutex_lock(&marg->sh->consumer_lock);

        if(is_empty(marg->sh->queue) && (marg->sh->max_requests <= marg->sh->requests_produced))
            break;

        RequestType requestType = dequeue(queue);
        if(requestType == End) {
            pthread_mutex_unlock(&marg->sh->consumer_lock);
            break;
        }
        marg->sh->consumed[marg->mcType][requestType]++;

        if(requestType == Bitcoin && marg->sh->produced[requestType] == 6)
            sem_post(&marg->sh->sem_btc_cap);

        pthread_mutex_lock(&marg->sh->in_requestQueue_lock);
        marg->sh->inRequestQueue[requestType]--;
        pthread_mutex_unlock(&marg->sh->in_requestQueue_lock);


        pthread_mutex_unlock(&marg->sh->consumer_lock);

        usleep(marg->wait_ms * 1000);
        // report
        report_request_removed(marg->mcType, requestType, marg->sh->consumed[marg->mcType], marg->sh->inRequestQueue);
    }

    sem_post(&marg->sh->barrier);
    pthread_exit(NULL);
}
