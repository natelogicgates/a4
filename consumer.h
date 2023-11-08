#ifndef CONSUMER_H
#define CONSUMER_H

#include "queue.h"
#include "tradecrypto.h"
#include "report.h"
#include "args.h"

#include <unistd.h>

void *consumer(void *arg);

#endif // CONSUMER_H
