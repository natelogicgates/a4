#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "tradecrypto.h"
#include "producer.h"
#include "consumer.h"
#include "report.h"
#include "args.h"

int main(int argc, char *argv[]) {
    int opt;
    int n = 120;    // number of total requests
    int x = 0;      // miliseconds to wait
    int y = 0;
    int b = 0;
    int e = 0;
    //optional args
    while ((opt = getopt(argc, argv, "n:x:y:b:e:")) != -1) {
        switch (opt) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'x':
                x = atoi(optarg);
                break;
            case 'y':
                y = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 'e':
                e = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-n N] [-x X] [-y Y] [-b B] [-e E]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }


    struct thread_args_shared *sh = malloc(sizeof (struct thread_args_shared));
    sh->queue = malloc(sizeof (RequestQueue));
    init_request_queue(sh->queue);
    sem_init(&sh->sem_btc_cap, 0, 0);

    sh->consumed = malloc(sizeof (int *) * 2);
    for(int i=0; i<2; ++i) {
        sh->consumed[i] = malloc(sizeof (int) * 2);
        sh->consumed[i][0] = sh->consumed[i][1] = 0;
    }
    sh->produced = malloc(sizeof (int) * 2);
    sh->produced[0] = sh->produced[1] = 0;

    sh->inRequestQueue = malloc(sizeof (int ) * 2);
    sh->inRequestQueue[0] = sh->inRequestQueue[1] = 0;

    pthread_mutex_init(&sh->in_requestQueue_lock, NULL);
    pthread_mutex_init(&sh->producer_lock, NULL);
    pthread_mutex_init(&sh->consumer_lock, NULL);

    sh->max_requests = n;
    sh->requests_produced = 0;

    sem_init(&sh->barrier, 0, 0);

    struct thread_args *args1, *args2, *args3, *args4;
    args1 = malloc(sizeof (struct thread_args));
    args2 = malloc(sizeof (struct thread_args));
    args3 = malloc(sizeof (struct thread_args));
    args4 = malloc(sizeof (struct thread_args));

    args1->sh = sh;
    args2->sh = sh;
    args3->sh = sh;
    args4->sh = sh;

    args1->mrType = Bitcoin;
    args1->wait_ms = b;

    args2->mrType = Ethereum;
    args2->wait_ms = e;

    args3->mcType = BlockchainX;
    args3->wait_ms = x;

    args4->mcType = BlockchainY;
    args4->wait_ms = y;


    // init 2 producer
    pthread_t producers[2], consumers[2];
    pthread_create(&producers[0], NULL, producer, args1);
    pthread_create(&producers[1], NULL, producer, args2);
    pthread_create(&consumers[0], NULL, consumer, args3);
    pthread_create(&consumers[1], NULL, consumer, args4);

    sem_wait(&sh->barrier);
    //reporting output of producer and consumer
    report_production_history(sh->produced, sh->consumed);
    return 0;
}


