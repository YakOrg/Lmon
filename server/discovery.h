//
// Created by dragon on 10.05.19.
//

#ifndef LMON_DISCOVERY_H
#define LMON_DISCOVERY_H

#include <netinet/in.h>

void startServer(int httpPort);

typedef struct Agent {
    struct sockaddr_in ip;
    int port;
} agent;

#endif //LMON_DISCOVERY_H
