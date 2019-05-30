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

const char *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        char *addr = malloc(INET_ADDRSTRLEN);
        memset(addr, 0, INET_ADDRSTRLEN);
        struct sockaddr_in *sa4 = (struct sockaddr_in *) sa;
        return inet_ntop(AF_INET, &sa4->sin_addr, addr, INET_ADDRSTRLEN);
    } else {
        char *addr = malloc(INET6_ADDRSTRLEN);
        memset(addr, 0, INET6_ADDRSTRLEN);
        struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *) sa;
        return inet_ntop(sa->sa_family, &sa6->sin6_addr, addr, INET6_ADDRSTRLEN);
    }
}

void send_post(char *server_url, char *endpoint) {
    CURL *curl = curl_easy_init();
    char *data = malloc((11 + strlen(endpoint)) * sizeof(char));
    sprintf(data, "endpoint=%s", endpoint);
    log_trace("POST (endpoint=%s) --> %s", endpoint, server_url);
    int res;

    if (curl) {
        /* instead of "SERVER_URL" you can try str_addr
         * and it`s will be work */
        curl_easy_setopt(curl, CURLOPT_URL, server_url);
        /*OK, let`s specify the POST data*/
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

/*return ip of the server(and so on...)*/
void start_broadcast_listener(int port, void *ptr) {
    int sock_fd = 0;
    struct addrinfo hints, *addr_info, *p;
    struct sockaddr_storage remote_addr;
    socklen_t addr_len;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; /*IPV4 or IPV6*/
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    char char_port[10];
    sprintf(char_port, "%d", port);

    if (getaddrinfo(NULL, char_port, &hints, &addr_info) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    /*cycle through all the results, bind to the first possible*/
    for (p = addr_info; p != NULL; p = p->ai_next) {
        if ((sock_fd = socket(p->ai_family, p->ai_socktype,
                              p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock_fd);
            perror("listener: bind");
            continue;
        }

        char *addr = inet_ntoa(((struct sockaddr_in *) p->ai_addr)->sin_addr);
        log_trace("Listen broadcast on %s", addr);
        break;
    }

    if (p == NULL) {
        perror("listener");
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof remote_addr;


    for (;;) {
        char *recvString = malloc(256 * sizeof(char));
        if ((recvfrom(sock_fd, recvString, 255 * sizeof(char), 0,
                      (struct sockaddr *) &remote_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(1);
        }

        recvString[255] = '\0';

        const char *ip = get_in_addr((struct sockaddr *) &remote_addr);
        log_trace("%s --> brd (size: %d, content: %s)", ip, strlen(recvString), recvString);


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
        char *server_url = malloc((36 + strlen(recvString)) * sizeof(char));
        sprintf(server_url, "http://%s:%s/agents/add", ip, recvString);
        free((char *) ip);
        send_post(server_url, endpoint);
        free(recvString);
    }
}

void start_agent(int http_port, char *server_url) {
    log_info("Starting agent with http server on 0.0.0.0:%d", http_port);
    start_metrics_server(http_port);
    start_broadcast_listener(1973, &http_port);
}