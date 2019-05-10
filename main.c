#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agent/agent.h"
#include "server/discovery.h"

void printUsage() {
    printf(
            "NAME:\n   lmon - monitoring, but small and simple\n\n"
            "USAGE:\n   lmon [commands] [arguments...]\n\n"
            "COMMANDS:\n   server   Run management server\n   agent    Run node agent\n\n'"
            "ARGS:\n   --http-port <PORT> http server port\n"
    );
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printUsage();
        return 0;
    }

    int httpPort = 8080;
    char *enptr;

    for (int i = 0; i < argc; i++)
        if (strcmp(argv[i], "--http-port") == 0 && i + 1 < argc)
            httpPort = strtol(argv[i + 1], &enptr, 10);


    if (strcmp(argv[1], "server") == 0)
        startServer(httpPort);
    else if (strcmp(argv[1], "agent") == 0)
        startAgent(httpPort);
    else printUsage();

    return 0;
}