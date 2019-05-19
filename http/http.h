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
#include "libhttp.h"

#define HEADER \
    "HTTP/1.1 200 OK\r\n" \
    "Content-Type: application/json; charset=UTF-8\r\n" \
    "Content-Length: %li\r\n" \
    "\r\n%s"

void start_http_server_no_blocking(char *(*genPage)(), int port);

#endif //LMON_HTTP_H
