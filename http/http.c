//
// Created by dragon on 26.05.19.
//
#include "http.h"

int send_page(struct MHD_Connection *connection,
              char *text,
              char *content_type,
              unsigned int status_code) {
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(text), text, MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, content_type);
    int ret = MHD_queue_response(connection, status_code, response);
    MHD_destroy_response(response);
    return ret;
}

int send_json(struct MHD_Connection *connection, char *json) {
    return send_page(connection, json, CONTENT_TYPE_JSON, MHD_HTTP_OK);
}

int bad_request(struct MHD_Connection *connection) {
    return send_page(connection, BAD_REQUEST, CONTENT_TYPE_TEXT, MHD_HTTP_BAD_REQUEST);
}

int not_found(struct MHD_Connection *connection) {
    return send_page(connection, NOT_FOUND, CONTENT_TYPE_TEXT, MHD_HTTP_NOT_FOUND);
}