//
// Created by dragon on 10.05.19.
//

#include "discovery.h"
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

pthread_t tid;
agent *agents;

void *sendBroadcast(void *arg) {
    int sock;                         /* Socket */
    struct sockaddr_in broadcastAddr; /* Broadcast address */
    char *broadcastIP;                /* IP broadcast address */
    unsigned short broadcastPort;     /* Server port */
    char *sendString;                 /* String to broadcast */
    int broadcastPermission;          /* Socket opt to set permission to broadcast */
    unsigned int sendStringLen;       /* Length of string to broadcast */

    broadcastIP = getIP();            /* First arg:  broadcast IP address */
    broadcastPort = 1973;             /* Second arg:  broadcast port */
    sendString = getIP();             /* Third arg:  string to broadcast */

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        err(1, "Socket error");
    }
    /* Set socket to allow broadcast */
    broadcastPermission = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,
                   sizeof(broadcastPermission)) < 0) {
        err(1, "broadcast error");
    }

    /* Construct local address structure */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
    broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP);/* Broadcast IP address */
    broadcastAddr.sin_port = htons(broadcastPort);         /* Broadcast port */

    sendStringLen = strlen(sendString);  /* Find length of sendString */
    for (;;) /* Run forever */
    {
        //printf("Broadcast send\n");
        /* Broadcast sendString in datagram to clients every 3 seconds*/
        if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *)
                &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen)
            err(1, "sendString err");

        sleep(3);   /* Avoids flooding the network */
    }
    /* NOT REACHED */
}

void startBroadcastSender() {
    int err = pthread_create(&tid, NULL, &sendBroadcast, NULL);
    if (err != 0)
        printf("can't create thread :[%s]", strerror(err));
    else
        printf("Thread created successfully\n");
}

char *genFrontEnd() {
    return "\"Cluster Frontend\"";
}

void startServer(int httpPort) {
    startBroadcastSender();
    // TODO : Receive node ip from http header
    startHttpServer(genFrontEnd, httpPort);
}