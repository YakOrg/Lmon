#include "main.h"

void daemon_start_agent(int port, char *serverURL) {
    if(daemon(0,0) != 0){
        log_error("can't start agent as daemon");
        exit(EXIT_FAILURE);
    }
    start_agent(port, serverURL);
}

void printUsage() {
    printf(USAGE);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printUsage();
        return 0;
    }

    int http_port = 8080;
    char *enptr;
    int as_daemon = 0;
    int run_agent = 0;

    char server_url[60];

    log_set_level(LOG_INFO);

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--http-port") == 0 && i + 1 < argc)
            http_port = strtol(argv[++i], &enptr, 10);
        if (strcmp(argv[i], "--daemon") == 0)
            as_daemon = 1;
        if (strcmp(argv[i], "--debug") == 0)
            log_set_level(LOG_DEBUG);
        if (strcmp(argv[i], "--trace") == 0)
            log_set_level(LOG_TRACE);
        if (strcmp(argv[i], "--with-agent") == 0)
            run_agent = 1;
        if (strcmp(argv[i], "--server") == 0 && i + 1 < argc && strlen(argv[i + 1]) < 60)
            strcpy(server_url, argv[++i]);
    }

    if (strcmp(argv[1], "server") == 0) {
        start_server(http_port, run_agent);
    } else if (strcmp(argv[1], "agent") == 0) {
        if (as_daemon == 1)
            daemon_start_agent(http_port, server_url);
        else
            start_agent(http_port, server_url);
    } else printUsage();

    return 0;
}