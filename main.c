#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http.h"
#include "metrics.h"
#include "templates.h"

char *genPage() {
    char *json = malloc(2048);
    char *baseBlock = malloc(1024);
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

    sprintf(baseBlock, BASE_TEMPLATE, hostname, kernelVersion(), cpuLoadAvg, memTotal, memUsed, swapTotal, swapUsed);

    strcat(json, objectStart);
    strcat(json, baseBlock);
    strcat(json, delimiter);

    strcat(json, DRIVES_OBJ);
    strcat(json, arrayStart);
    for (drive *d = getDrives(); d->blockPath != NULL; d++) {
        char *drive = malloc(strlen(DRIVE_TEMPLATE) + 50);
        sprintf(drive, DRIVE_TEMPLATE, d->blockPath, d->mountPoint, d->size, d->usage);
        strcat(json, drive);
        free(drive);
        if ((d + 1)->blockPath != NULL) strcat(json, delimiter);
    }
    strcat(json, arrayEnd);


    strcat(json, objectEnd);

    return json;
}

int main() {
    startHttpServer(genPage, 8080);
}