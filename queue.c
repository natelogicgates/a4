#include "queue.h"

void init_request_queue(RequestQueue *queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
    pthread_cond_init(&queue->not_full, NULL);
}

void enqueue(RequestQueue *queue, RequestType request) {
    pthread_mutex_lock(&queue->mutex);
    if (queue->count <= 0) {
        pthread_cond_signal(&queue->not_full);
    }
    if (queue->count >= MAX_QUEUE_SIZE) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }
    queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
    queue->requests[queue->rear] = request;
    queue->count++;
    pthread_mutex_unlock(&queue->mutex);
}

RequestType dequeue(RequestQueue *queue) {
    pthread_mutex_lock(&queue->mutex);
    if(queue->count >= MAX_QUEUE_SIZE) {
        pthread_cond_signal(&queue->not_empty);
    }
    if (queue->count <= 0) {
        // queue is empty, wait for a producer to add a request
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }
    RequestType request = queue->requests[queue->front];
    queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
    queue->count--;
    pthread_mutex_unlock(&queue->mutex);
    return request;
}

int is_full(const RequestQueue *queue) {
    return (queue->count >= MAX_QUEUE_SIZE);
}

void destroy_request_queue(RequestQueue *queue) {
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->not_empty);
}

int is_empty(const RequestQueue *queue) {
    return !queue->count;
}
