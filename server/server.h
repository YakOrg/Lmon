//
// Created by dragon on 10.05.19.
//

#ifndef LMON_SERVER_H
#define LMON_SERVER_H


#include "../agent/metrics/metrics.h"
#include "../http/http.h"
#include <microhttpd.h>

void startServer(int httpPort);

typedef struct Agent {
    struct sockaddr_in ip;
    int port;
    struct agent *next;
    struct agent *first;
} agent;

#endif //LMON_SERVER_H
