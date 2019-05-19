#include "main.h"

void daemon_start_agent(int port){
    if(daemon(0,0) != 0){
        log_error("can't start agent as daemon");
        exit(EXIT_FAILURE);
    }
    startAgent(port);
}

void printUsage() {
    printf(USAGE);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printUsage();
        return 0;
    }

    int httpPort = 8080;
    char *enptr;
    int as_daemon = 0;

    log_set_level(LOG_INFO);

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--http-port") == 0 && i + 1 < argc)
            httpPort = strtol(argv[i + 1], &enptr, 10);
        if (strcmp(argv[i], "--daemon") == 0)
            as_daemon = 1;
        if (strcmp(argv[i], "--debug") == 0)
            log_set_level(LOG_TRACE);
    }

    if (strcmp(argv[1], "server") == 0) {
        startServer(httpPort);
    } else if (strcmp(argv[1], "agent") == 0) {
        if (as_daemon == 1)
            daemon_start_agent(httpPort);
        else
            startAgent(httpPort);
    } else printUsage();

    return 0;
}