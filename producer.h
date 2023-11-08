#ifndef PRODUCER_H
#define PRODUCER_H

#include "queue.h"
#include "tradecrypto.h"
#include "report.h"
#include "args.h"

#include <unistd.h>

void *producer(void *arg) ;

#endif // PRODUCER_H
