//
// Created by dragon on 10.05.19.
//

#ifndef LMON_HTTP_H
#define LMON_HTTP_H

#include <microhttpd.h>
#include <string.h>

#define NOT_FOUND "404 page not found"
#define BAD_REQUEST "Bad request"
#define CONTENT_TYPE_JSON "application/json; charset=utf-8"
#define CONTENT_TYPE_TEXT "text/plain; charset=utf-8"

int send_page(struct MHD_Connection *connection,
              char *text,
              char *content_type,
              unsigned int status_code);

int send_json(struct MHD_Connection *connection, char *json);

int bad_request(struct MHD_Connection *connection);

int not_found(struct MHD_Connection *connection);

#endif //LMON_HTTP_H
