#include <stdio.h>
#include <stdlib.h>

#include "http.h"
#include "metrics.h"

#define TEMPLATE\
 "{\"info\": {\"hostname\": \"%s\"}, "\
 "\"cpu\": {\"load_avg\": %.2f}, "\
 "\"memory\": {\"size\": %d, \"usage\": %d, \"swap\": {\"size\": %d, \"usage\": %d}}}"

char *genPage() {
    char *template = malloc(sizeof(TEMPLATE) + 10);

    char *hostname = getHostname();
    double cpuLoadAvg = getCPULoadAvg();
    int memTotal = getMemAttr("MemTotal");
    int memAvailable = getMemAttr("MemAvailable");
    int memUsed = memTotal - memAvailable;

    int swapTotal = getMemAttr("SwapTotal");
    int swapFree = getMemAttr("SwapFree");
    int swapUsed = swapTotal - swapFree;

    sprintf(template, TEMPLATE, hostname, cpuLoadAvg, memTotal, memUsed, swapTotal, swapUsed);
    return template;
}

int main() {
    startHttpServer(genPage, 8080);
}