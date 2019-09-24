#include "agent.h"

static int handler(__attribute__((unused)) void *cls,
                   struct MHD_Connection *connection,
                   const char *url,
                   const char *method,
                   __attribute__((unused)) const char *version,
                   __attribute__((unused)) const char *upload_data,
                   __attribute__((unused)) size_t *upload_data_size,
                   __attribute__((unused)) void **ptr) {

    struct MHD_Response *response = NULL;
    int ret;
    if (0 != strcmp(method, "GET")) return MHD_NO;
    if (strcmp(url, "/") == 0) {

        metrics *m = get_all_metrics();
        json_t *json = make_json(m);
        char *str = json_dumps(json, JSON_REAL_PRECISION(3u) | JSON_COMPACT);
        json_decref(json);
        free_metrics(m);
        ret = send_json(connection, str);
    } else {
        ret = not_found(connection);
    }

    MHD_destroy_response(response);
    return ret;
}

struct MHD_Daemon *start_metrics_server(int http_port) {
    return MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD,
                            http_port,
                            0,
                            0,
                            &handler,
                            0,
                            MHD_OPTION_END);
}

size_t wr_data(
        __attribute__((unused)) void *buffer,
        size_t size,
        size_t nmemb,
        __attribute__((unused)) void *userp) {
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

void callback(char *message, char *ip, listener *listen) {
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
    int endpoint_port = *((int *) listen->args);
    char *endpoint = malloc((9 + strlen(m_address) + int_len(endpoint_port)) * sizeof(char));
    sprintf(endpoint, "http://%s:%d", m_address, endpoint_port);
    char *server_url = malloc((36 + strlen(message)) * sizeof(char));
    sprintf(server_url, "http://%s:%s/agents/add", ip, message);
    send_post(server_url, endpoint);
}

struct MHD_Daemon *mhd_daemon;

void stop() {
    MHD_stop_daemon(mhd_daemon);
}

void start_agent(int http_port) {
    log_info("Starting agent with http server on 0.0.0.0:%d", http_port);
    mhd_daemon = start_metrics_server(http_port);
    atexit(stop);
    signal(SIGTERM, stop);
    brd_listen_others(1973, callback, &http_port);
}