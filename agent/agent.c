//
// Created by dragon on 10.05.19.
//

#include "agent.h"

char *json_metrics() {
    metrics *m = get_all_metrics();
    json_t *json = make_json(m);
    free(m);
    return json_dumps(json, (size_t) JSON_REAL_PRECISION(3));
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
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

    if (0 != strcmp(method, "GET")) return MHD_NO;

    if (strcmp(url, "/") == 0) {
        char *content = json_metrics();
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

#pragma clang diagnostic pop

void start_metrics_server(int http_port) {
    MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD,
                     http_port,
                     NULL,
                     NULL,
                     &handler,
                     NULL,
                     MHD_OPTION_END);
}

size_t wr_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    return size * nmemb;
}

void send_post(char *server_url, char *endpoint) {
    CURL *curl = curl_easy_init();
    char *data = malloc((11 + strlen(endpoint)) * sizeof(char));
    sprintf(data, "endpoint=%s", endpoint);
    log_trace("POST (endpoint=%s) --> %s", endpoint, server_url);
    int res;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, server_url);
        curl_easy_setopt (curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wr_data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void callback(char *message, char *ip, void *ptr) {
    network_interface *network_interfaces = get_interfaces();
    int mk = 0;
    char *m_address = malloc(17 * sizeof(char));
    for (network_interface *iter = network_interfaces; iter; iter = iter->next)
        for (net_address *address = iter->addresses; address; address = address->next)
            for (int i = 0, k = 0; i < strlen(address->ip_address) && i < strlen(ip); i++) {
                if (address->ip_address[i] == ip[i]) k++;
                if (k > mk) {
                    mk = k;
                    strcpy(m_address, address->ip_address);
                }
            }
    m_address[16] = '\0';
    int endpoint_port = *((int *) ptr);
    char *endpoint = malloc((9 + strlen(m_address) + int_len(endpoint_port)) * sizeof(char));
    sprintf(endpoint, "http://%s:%d", m_address, endpoint_port);
    char *server_url = malloc((36 + strlen(message)) * sizeof(char));
    sprintf(server_url, "http://%s:%s/agents/add", ip, message);
    send_post(server_url, endpoint);
}

void start_agent(int http_port, char *server_url) {
    log_info("Starting agent with http server on 0.0.0.0:%d", http_port);
    start_metrics_server(http_port);
    pthread_t pthread = *BRD_listen(1973, callback, &http_port);
    pthread_join(pthread, NULL);
}