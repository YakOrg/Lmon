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

void start_metrics_server(int http_port) {
    struct MHD_Daemon *d;
    d = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD,
                         http_port,
                         NULL,
                         NULL,
                         &handler,
                         NULL,
                         MHD_OPTION_END);
    getchar();
    MHD_stop_daemon(d);
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

/*return ip of the server(and so on...)*/
char *start_broadcast_listener(char *port) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int res;
    int numbytes;
    struct sockaddr_storage remote_addr;
    socklen_t addr_len;
    char str_addr[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; /*IPV4 or IPV6*/
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &servinfo) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    /*cycle through all the results, bind to the first possible*/
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        perror("listener");
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof remote_addr;
    /*you can remove numbytes (would it be useful in the future?)*/
    if ((numbytes = recvfrom(sockfd, NULL, INET_ADDRSTRLEN - 1, 0,
                             (struct sockaddr *) &remote_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }

    inet_ntop(remote_addr.ss_family,
              get_in_addr((struct sockaddr *) &remote_addr),
              str_addr, sizeof(str_addr));

    /*now str_addr contains the ip of the server(string)*/

    CURL *curl;

    curl = curl_easy_init();

    if (curl) {
        /*instead of "SERVER_URL" you can try str_addr
         * and it`s will be work
        */
        curl_easy_setopt(curl, CURLOPT_URL, "SERVER_URL");
        /*OK, let`s specify the POST data*/
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "ip and port");

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return str_addr;
}

void start_agent(int http_port, char *server_url) {
    start_broadcast_listener("1973");
    start_metrics_server(http_port);
}