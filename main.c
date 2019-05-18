#include "main.h"

void start_agent_as_daemon(int port){
    /*TODO*/
    /*there is should be a daemon code*/
    startAgent(port);
};

void printUsage() {
    printf(
            "NAME:\n   lmon - monitoring, but small and simple\n\n"
            "USAGE:\n   lmon [commands] [arguments...]\n\n"
            "COMMANDS:\n   server   Run management server\n   agent    Run node agent\n\n"
            "ARGS:\n   --http-port <PORT> http server port\n   --help             show this help\n   --daemon         start agent as daemon"
    );
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printUsage();
        return 0;
    }

    int httpPort = 8080;
    char *enptr;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--http-port") == 0 && i + 1 < argc)
            httpPort = strtol(argv[i + 1], &enptr, 10);
        if(strcmp(argv[i], "--daemon") == 0)
           start_agent_as_daemon(httpPort);
    }

    if (strcmp(argv[1], "server") == 0)
        startServer(httpPort);
    else if (strcmp(argv[1], "agent") == 0)
        startAgent(httpPort);
    else printUsage();

    return 0;
}