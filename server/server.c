//
// Created by dragon on 10.05.19.
//

#include "server.h"

static int handler(void *cls,
                   struct MHD_Connection *connection,
                   const char *url,
                   const char *method,
                   const char *version,
                   const char *upload_data,
                   size_t *upload_data_size,
                   void **ptr) {
    struct MHD_Response *response = NULL;
    int ret;

    struct Agent *agents = (agent *) cls;

    if (0 != strcmp(method, "GET") && 0 != strcmp(method, "POST"))
        return MHD_NO;

    if (strcmp(url, "/") == 0) {
        char *content = fetch_data_from_agents(agents);
        response = MHD_create_response_from_buffer(strlen(content), (void *) content, MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, "application/json; charset=utf-8");
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    } else if (strcmp(url, "/agents/add") == 0 && !strcmp(method, "POST")) {
        MHD_lookup_connection_value(connection, MHD_POSTDATA_KIND, NULL);
        // TODO : Get agent data from json received by POST
        char *content = "123";
        response = MHD_create_response_from_buffer(strlen(content), (void *) content, MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, "application/json; charset=utf-8");
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    } else {
        response = MHD_create_response_from_buffer(strlen(NOT_FOUND), NOT_FOUND, MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, "text/plain; charset=utf-8");
        ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    }

    MHD_destroy_response(response);
    return ret;
}

void run_httpd(int port, agent *agents) {
    struct MHD_Daemon *d;
    d = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD,
                         port,
                         NULL,
                         NULL,
                         &handler,
                         agents,
                         MHD_OPTION_END);
    getchar();
    MHD_stop_daemon(d);
}

void start_server(int httpPort) {
    agent *agents = NULL;
    run_httpd(httpPort, agents);
    run_blocking_brodcast();
}