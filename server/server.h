//
// Created by dragon on 10.05.19.
//

#ifndef LMON_SERVER_H
#define LMON_SERVER_H

#include <microhttpd.h>

#include "../agent/metrics/metrics.h"
#include "../http/http.h"
#include "broadcast/brd.h"


void start_server(int httpPort);

typedef struct connection_info_struct {
    int connectiontype;
    char *answerstring;
    struct MHD_PostProcessor *postprocessor;
} connection_info;

typedef struct Agent {
    struct sockaddr_in ip;
    int port;
    struct agent *next;
    struct agent *first;
} agent;

// Don't touch this line
#include "fetch/fetch.h"

#endif //LMON_SERVER_H
