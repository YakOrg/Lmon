#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agent/http/http.h"
#include "agent/metrics/metrics.h"
#include "agent/http/templates.h"

char *genPage() {
    char *json = malloc(sizeof(char) * 1500), baseBlock[600];
    // Info
    char *hostname = getHostname();
    // CPU
    double cpuLoadAvg = getCPULoadAvg();
    // Memory
    int memTotal = getMemAttr("MemTotal");
    int memAvailable = getMemAttr("MemAvailable");
    int memUsed = memTotal - memAvailable;
    // Swap
    int swapTotal = getMemAttr("SwapTotal");
    int swapFree = getMemAttr("SwapFree");
    int swapUsed = swapTotal - swapFree;

    sprintf(baseBlock, BASE_TEMPLATE, hostname, getIP(), kernelVersion(), cpuLoadAvg, memTotal, memUsed, swapTotal,
            swapUsed);

    strcat(json, objectStart);
    strcat(json, baseBlock);
    strcat(json, delimiter);

    strcat(json, DRIVES_OBJ);
    strcat(json, arrayStart);
    drive *iter = getDrives();
    while (!iter->end) {
        char drive[300];
        sprintf(drive, DRIVE_TEMPLATE, iter->blockPath, iter->mountPoint, iter->size, iter->usage);
        strcat(json, drive);
        if (!(++iter)->end) {
            strcat(json, delimiter);
        }
    }
    strcat(json, arrayEnd);
    strcat(json, objectEnd);

    return json;
}

int main(int argc, char **argv) {
    int port = 8080;
    if (argc > 1) {
        char *endpoint;
        int someArg = strtol(argv[1], &endpoint, 10);
        port = someArg > 999 ? someArg : port;
    }
    startHttpServer(genPage, port);
}