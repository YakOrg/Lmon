//
// Created by dragon on 10.05.19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agent.h"
#include "../http/http.h"
#include "metrics/metrics.h"
#include "templates.h"

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

void startAgent(int httpPort) {
    startHttpServer(genPage, httpPort);
}