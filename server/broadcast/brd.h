//
// Created by dragon on 24.05.19.
//

#ifndef LMON_BRD_H
#define LMON_BRD_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <pthread.h>
#include <libbrd.h>

#include "../../log/log.h"
#include "../../agent/metrics/metrics.h"

void run_blocking_broadcast(char *sendString);

#endif //LMON_BRD_H
