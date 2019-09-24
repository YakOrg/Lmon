#include "main.h"

void daemon_start_agent(int port) {
    if(daemon(0,0) != 0){
        log_error("can't start agent as daemon");
        exit(EXIT_FAILURE);
    }
    start_agent(port);
}

void printUsage() {
    printf(USAGE);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printUsage();
        return 0;
    }

    log_set_level(LOG_INFO);

    int http_port = 8080;
    int as_daemon = 0;
    int run_agent = 0;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--http-port") == 0 && i + 1 < argc) {
            char *end_ptr;
            long port = strtol(argv[++i], &end_ptr, 10);
            if (1 < port && port < 65535)
                http_port = (int) port;
            else {
                log_error("Invalid port");
                return 0;
            }
        }
        if (strcmp(argv[i], "--daemon") == 0)
            as_daemon = 1;
        if (strcmp(argv[i], "--debug") == 0)
            log_set_level(LOG_DEBUG);
        if (strcmp(argv[i], "--trace") == 0)
            log_set_level(LOG_TRACE);
        if (strcmp(argv[i], "--with-agent") == 0)
            run_agent = 1;
    }

    if (strcmp(argv[1], "server") == 0) {
        start_server(http_port, run_agent);
    } else if (strcmp(argv[1], "agent") == 0) {
        if (as_daemon == 1)
            daemon_start_agent(http_port);
        else
            start_agent(http_port);
    } else printUsage();

    return 0;
}