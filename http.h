//
// Created by dragon on 10.05.19.
//

#ifndef LMON_HTTP_H
#define LMON_HTTP_H

#define HEADER_READ_SIZE 512
#define BASE_BODY "HTTP/1.1 200 OK\r\nStatus: 200 OK\r\nContent-Type: application/json; charset=UTF-8\r\n\r\n"

void startServer(char *(*genPage)(), int port);

#endif //LMON_HTTP_H
