//
// Created by dragon on 10.05.19.
//

#include "http.h"

static int begin_request_handler(struct lh_ctx_t *ctx, struct lh_con_t *conn, void *data) {
    char *resp = ((char *(*)()) data)();
    httplib_printf(ctx, conn, HEADER, strlen(resp) * sizeof(char), resp);
    free(resp);
    return 1;
}

void start_http_server_no_blocking(char *(*src)(), int port) {
    struct lh_ctx_t *ctx;
    struct lh_clb_t callbacks;

    char str_port[16];
    sprintf(str_port, "%d", port);
    log_info("HTTP server started on port %d", port);

    // List of options. Last element must be NULL.
    struct lh_opt_t opt;
    opt.name = "listening_ports";
    opt.value = str_port;

    // Prepare callbacks structure. We have only one callback, the rest are NULL.
    memset(&callbacks, 0, sizeof(callbacks));
    // Start the web server.
    ctx = httplib_start(&callbacks, NULL, &opt);
    httplib_set_request_handler(ctx, "/", begin_request_handler, src);
}