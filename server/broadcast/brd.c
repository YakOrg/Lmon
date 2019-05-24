//
// Created by dragon on 24.05.19.
//

#include "brd.h"

void *sendBroadcast() {
    int sock;                         /* Socket */
    struct sockaddr_in broadcastAddr; /* Broadcast address */
    char *broadcastIP;                /* IP broadcast address */
    unsigned short broadcastPort;     /* Server port */
    char *sendString;                 /* String to broadcast */
    int broadcastPermission;          /* Socket opt to set permission to broadcast */
    unsigned int sendStringLen;       /* Length of string to broadcast */

    broadcastIP = "";                 /* First arg:  broadcast IP address */
    broadcastPort = 1973;             /* Second arg:  broadcast port */
    sendString = "";                  /* Third arg:  string to broadcast */

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
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));       /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                     /* Internet address family */
    broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP); /* Broadcast IP address */
    broadcastAddr.sin_port = htons(broadcastPort);          /* Broadcast port */

    sendStringLen = strlen(sendString);  /* Find length of sendString */
    for (;;) {
        /* Broadcast sendString in datagram to clients every 10 seconds */
        if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *)
                &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen)
            err(1, "sendString err");

        sleep(10);   /* Avoids flooding the network */
    }
}

void startBroadcastSender() {

}