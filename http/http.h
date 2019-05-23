//
// Created by dragon on 10.05.19.
//

#ifndef LMON_HTTP_H
#define LMON_HTTP_H

#include <unistd.h>
#include <arpa/inet.h>
#include <err.h>
#include <string.h>
#include <malloc.h>
#include "../log/log.h"

#define NOT_FOUND "404 page not found"

#define HEADER \
    "HTTP/1.1 200 OK\r\n" \
    "Content-Type: application/json; charset=UTF-8\r\n" \
    "Content-Length: %li\r\n" \
    "\r\n%s"

#define HEADER_NF \
    "HTTP/1.1 404 Not Found\r\n" \
    "Content-Type: text/plain; charset=UTF-8\r\n\r\n404 page not found\r\n"

void start_http_server_no_blocking(char *(*genPage)(), int port);

#endif //LMON_HTTP_H
