//
// Created by dragon on 24.05.19.
//

#include "brd.h"

typedef struct BrdArgs {
    char *send_str;
    char *net;
} brd_args;

void *brd(void *ptr) {
    brd_args *args = (brd_args *) ptr;
    int sock;                         /* Socket */
    struct sockaddr_in broadcastAddr; /* Broadcast address */
    char *broadcastIP;                /* IP broadcast address */
    unsigned short broadcastPort;     /* Server port */
    int broadcastPermission;          /* Socket opt to set permission to broadcast */
    unsigned int sendStringLen;       /* Length of string to broadcast */

    broadcastIP = args->net;                /* First arg:  broadcast IP address */
    broadcastPort = 1973;             /* Second arg:  broadcast port */

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
    sendStringLen = strlen(args->send_str);                     /* Find length of sendString */
    for (;;) {
        log_trace("brd -> %s:%d (body: %s, size: %d)", broadcastIP, broadcastPort, args->send_str, sendStringLen);
        /* Broadcast sendString in datagram to clients every 10 seconds */
        if (sendto(sock, args->send_str, sendStringLen * sizeof(char), 0,
                   (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen) {
            err(1, "sendString err");
        }
        sleep(10);   /* Avoids flooding the network */
    }
}

void run_blocking_broadcast(char *sendString) {
    pthread_t *thread = NULL;

    network_interface *net_interfaces = get_interfaces();
    for (network_interface *iter = net_interfaces; iter; iter = iter->next)
        for (net_address *address = iter->addresses; address; address = address->next)
            if (address->type == IPV4) {
                char *ip = NULL;
                for (int i = 0, dot_n = 0; i < strlen(address->ip_address); i++) {
                    if (address->ip_address[i] == '.') dot_n++;
                    if (dot_n == 3) {
                        ip = malloc((i + 3) * sizeof(char));
                        memcpy(ip, address->ip_address, (i + 1) * sizeof(char));
                        ip[i + 1] = '0';
                        ip[i + 2] = '\0';
                        break;
                    }
                }
                if (ip) {
                    brd_args *args = malloc(sizeof(brd_args));
                    args->net = ip;
                    args->send_str = sendString;
                    thread = malloc(sizeof(pthread_t));
                    pthread_create(thread, NULL, brd, args);
                }
            }
    if (thread)
        pthread_join(*thread, NULL);
}