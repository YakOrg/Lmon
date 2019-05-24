//
// Created by dragon on 10.05.19.
//

#include "agent.h"

metrics *get_all_metrics() {
    metrics *m = malloc(sizeof(metrics));
    get_base_metrics(m);
    get_advanced_metrics(m);
    return m;
}

char *json_metrics() {
    metrics *m = get_all_metrics();
    json_t *json = make_json(m);
    free(m);
    return json_dumps(json, JSON_REAL_PRECISION(3));
}

static int handler(void *cls,
                   struct MHD_Connection *connection,
                   const char *url,
                   const char *method,
                   const char *version,
                   const char *upload_data,
                   size_t *upload_data_size,
                   void **ptr) {

    const char *page = cls;
    struct MHD_Response *response = NULL;
    int ret;

    if (0 != strcmp(method, "GET"))
        return MHD_NO; /* unexpected method */

    if (strcmp(url, "/") == 0) {
        response = MHD_create_response_from_buffer(strlen(page), (void *) page, MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, "application/json");
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    } else {
        response = MHD_create_response_from_buffer(strlen(NOT_FOUND), NOT_FOUND, MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, "text/plain; charset=utf-8");
        ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    }

    MHD_destroy_response(response);
    return ret;
}

void start_metrics_server(int http_port) {
    struct MHD_Daemon *d;
    d = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD,
                         http_port,
                         NULL,
                         NULL,
                         &handler,
                         json_metrics(),
                         MHD_OPTION_END);
    getchar();
    MHD_stop_daemon(d);
}

void start_broadcast_listener(int port)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int res;
    int numbytes;
    struct sockaddr_storage remote_addr;
    socklen_t addr_len;
    char s[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET; /*IPV4 only(temporarily)*/
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags    = AI_PASSIVE;

    if(getaddrinfo(NULL, port, &hints, &servinfo) != 0){
        /*TODO*/
    }
}

void start_agent(int http_port, char *server_url) {
    start_metrics_server(http_port);
}