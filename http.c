//
// Created by dragon on 10.05.19.
//

#include "http.h"

#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <err.h>
#include <string.h>
#include <malloc.h>

void startServer(char *(*genPage)(), int port) {
    int one = 1, client_fd;
    struct sockaddr_in svr_addr, cli_addr;
    socklen_t sin_len = sizeof(cli_addr);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) err(1, "can't open socket");
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = INADDR_ANY;
    svr_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
        close(sock);
        err(1, "Can't bind");
    }

    listen(sock, 5);

    for (;;) {
        client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);

        // Read client headers
        char buf[HEADER_READ_SIZE];
        int error = recv(client_fd, buf, HEADER_READ_SIZE, 0) < 0;
        //free(buf);
        if (error) continue;

        // Invoke function and generate response
        char *generated = genPage();
        char *resp = malloc(strlen(generated) + strlen(BASE_BODY));
        sprintf(resp, "%s%s\n", BASE_BODY, generated);

        // Write response and close socket
        write(client_fd, resp, strlen(resp));
        close(client_fd);

        // Clear memory
        free(generated);
        free(resp);
    }
}