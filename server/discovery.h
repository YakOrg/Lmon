//
// Created by dragon on 10.05.19.
//

#ifndef LMON_DISCOVERY_H
#define LMON_DISCOVERY_H

#include "../agent/metrics/metrics.h"
#include "../http/http.h"

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <err.h>
#include <pthread.h>
#include <netinet/in.h>

void startServer(int httpPort);

typedef struct Agent {
    struct sockaddr_in ip;
    int port;
} agent;

#endif //LMON_DISCOVERY_H
