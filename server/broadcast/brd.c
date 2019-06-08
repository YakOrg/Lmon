//
// Created by dragon on 24.05.19.
//

#include "brd.h"

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

                if (ip)
                    thread = BRD_send(sendString, ip, 1973, 10);
            }

    if (thread)
        pthread_join(*thread, NULL);
}