//
// Created by dragon on 10.05.19.
//

#include "http.h"

static int begin_request_handler(struct lh_ctx_t *ctx, struct lh_con_t *conn) {
    const struct lh_rqi_t *request_info = httplib_get_request_info(conn);
    char content[100];

    // Prepare the message we're going to send
    int content_length = snprintf(content, sizeof(content),
                                  "Hello from libhttp! Remote port: %d",
                                  request_info->remote_port);

    // Send HTTP reply to the client
    httplib_printf(ctx, conn, "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/plain\r\n"
                              "Content-Length: %d\r\n"        // Always set Content-Length
                              "\r\n"
                              "%s", content_length, content);

    return 1;
}

void startHttpServer(char *(*genPage)(), int port) {
    struct lh_ctx_t *ctx;
    struct lh_clb_t callbacks;

    // List of options. Last element must be NULL.
    struct lh_opt_t opt;
    opt.name = "listening_ports";
    opt.value = "8080";

    // Prepare callbacks structure. We have only one callback, the rest are NULL.
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.begin_request = begin_request_handler;

    // Start the web server.
    ctx = httplib_start(&callbacks, NULL, &opt);
    getchar();
}